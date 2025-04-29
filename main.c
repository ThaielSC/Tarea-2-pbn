#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ===DECLARACION DE INDICES=== */
const int SONG_ID = 0, SONG_TITLE = 1, ARTIST = 2, ALBUM = 3, GENRE = 4,
          RELEASE_DATE = 5, DURATION = 6, POPULARITY = 7, STREAM = 8,
          LANGUAGE = 9, EXPLICIT_CONTENT = 10, LABEL = 11, COMPOSER = 12,
          PRODUCER = 13, COLLABORATION = 14;

/* ===DECLARACION DE TIPOS=== */
typedef char ISO[3];
typedef char ID[8];
typedef char bool;

const bool True = 1;
const bool False = 0;

typedef struct
{
  int year;
  char month;
  char day;
} Date;

typedef struct
{
  ID id;
  char *title;
  char *artist;
  char *album;
  char *genre;
  Date release_date;
  float duration;
  char popularity;
  int stream;
  char *language;
  bool explicit_content;
  char *label;
  char *composer;
  char *producer;
  char *collaboration;
} song;

/* pool generico de strings */
typedef struct // TODO: Añadir fechas al pool
{
  char **strings;
  int filled;
  int capacity;
} StringPool;

/* se utiliza para hacer un pool con de los elementos que se pueden repetir y
 * así evitar crear copias de esto utilizando punteros a ellos desde song*/
typedef struct // Como almacena valores unicos puede ser util para crear filtros
{
  StringPool artist_pool;
  StringPool album_pool;
  StringPool genre_pool;
  StringPool label_pool;
  StringPool language_pool;
  StringPool composer_pool;
  StringPool producer_pool;
  StringPool collaboration_pool;
} songPool;

/* ===UTILITARIOS=== */
void show_usage_error(const char *__name__)
{
  fprintf(stderr, "Error: You must provide the CSV file as an argument.\n");
  fprintf(stderr, "Usage: %s <filename.csv>\n", __name__);
  exit(EXIT_FAILURE);
}

char *strdup(const char *s)
{
  size_t len = strlen(s) + 1;
  char *copy = malloc(len);
  if (copy)
    strcpy(copy, s);
  return copy;
}

void remove_newline(char *linea)
{
  size_t len = strlen(linea);
  if (len > 0 && linea[len - 1] == '\n')
    linea[len - 1] = '\0';
}

int count_delimiters(const char *linea, char separator)
{
  int count = 1;
  for (const char *c = linea; *c; c++)
    if (*c == separator)
      count++;
  return count;
}

char *create_field(const char *start, const char *end)
{
  size_t len = end - start;
  char *field = malloc(len + 1);
  if (!field)
    return NULL;
  strncpy(field, start, len);
  field[len] = '\0';
  return field;
}

char **split(const char *linea_original, int *count, char separator)
{
  char *linea = strdup(linea_original);
  if (!linea)
    return NULL;

  remove_newline(linea);
  int n = count_delimiters(linea, separator);
  int i = 0;

  char **result = malloc(sizeof(char *) * n);
  if (!result)
  {
    free(linea);
    return NULL;
  }

  const char *start = linea;
  for (const char *ptr = linea;; ptr++)
  {
    if (*ptr == separator || *ptr == '\0')
    {
      result[i++] = (ptr == start) ? strdup("None") : create_field(start, ptr);
      if (*ptr == '\0')
        break;
      start = ptr + 1;
    }
  }

  *count = n;
  free(linea);
  return result;
}

/* ===Funciones para String Interning=== */

int string_compare(const void *a, const void *b)
{
  return strcmp(*(const char **)a, *(const char **)b);
}

int binary_search_string(StringPool *pool, const char *str)
{
  int low = 0;
  int high = pool->filled - 1;

  while (low <= high)
  {
    int mid = low + (high - low) / 2;
    int cmp = strcmp(pool->strings[mid], str);

    if (cmp == 0)
      return mid;

    if (cmp < 0)
      low = mid + 1;
    else
      high = mid - 1;
  }

  return -1;
}

int find_insert_position(StringPool *pool, const char *str)
{
  int low = 0;
  int high = pool->filled - 1;

  while (low <= high)
  {
    int mid = low + (high - low) / 2;
    int cmp = strcmp(pool->strings[mid], str);

    if (cmp == 0)
      return mid;

    if (cmp < 0)
      low = mid + 1;
    else
      high = mid - 1;
  }

  return low;
}

char *intern_string(StringPool *pool, const char *str)
{
  if (!str || !*str)
    return NULL;

  int index = binary_search_string(pool, str);

  if (index >= 0)
    return pool->strings[index];

  if (pool->filled >= pool->capacity)
  {
    pool->capacity *= 2;
    pool->strings = realloc(pool->strings, sizeof(char *) * pool->capacity);
    if (!pool->strings)
    {
      fprintf(stderr, "Error: Fallo al redimensionar el string pool\n");
      exit(EXIT_FAILURE);
    }
  }

  int insert_pos = find_insert_position(pool, str);

  for (int i = pool->filled; i > insert_pos; i--)
  {
    pool->strings[i] = pool->strings[i - 1];
  }

  pool->strings[insert_pos] = strdup(str);
  if (!pool->strings[insert_pos])
  {
    fprintf(stderr, "Error: Fallo al duplicar el string\n");
    exit(EXIT_FAILURE);
  }

  pool->filled++;
  return pool->strings[insert_pos];
}

void init_string_pool(StringPool *pool)
{
  pool->capacity = 10;
  pool->filled = 0;
  pool->strings = malloc(sizeof(char *) * pool->capacity);
  if (!pool->strings)
  {
    fprintf(stderr, "Error: No se pudo inicializar el string pool\n");
    exit(EXIT_FAILURE);
  }
}

void free_string_pool(StringPool *pool)
{
  for (int i = 0; i < pool->filled; i++)
  {
    free(pool->strings[i]);
  }
  free(pool->strings);
  pool->strings = NULL;
  pool->filled = 0;
  pool->capacity = 0;
}

void init_song_pool(songPool *pool)
{
  init_string_pool(&pool->artist_pool);
  init_string_pool(&pool->album_pool);
  init_string_pool(&pool->genre_pool);
  init_string_pool(&pool->label_pool);
  init_string_pool(&pool->language_pool);
  init_string_pool(&pool->composer_pool);
  init_string_pool(&pool->producer_pool);
  init_string_pool(&pool->collaboration_pool);
}

void free_song_pool(songPool *pool)
{
  free_string_pool(&pool->artist_pool);
  free_string_pool(&pool->album_pool);
  free_string_pool(&pool->genre_pool);
  free_string_pool(&pool->label_pool);
  free_string_pool(&pool->language_pool);
  free_string_pool(&pool->composer_pool);
  free_string_pool(&pool->producer_pool);
  free_string_pool(&pool->collaboration_pool);
}

/* ===Cargar Canciones usando String Interning=== */
void set_basic_info(song *s, char **data, songPool *pool)
{
  strncpy(s->id, data[SONG_ID], 7);
  s->id[7] = '\0';

  s->title =
      strdup(data[SONG_TITLE]); // El título siempre es único, no se interna
  s->artist = intern_string(&pool->artist_pool, data[ARTIST]);
  s->album = intern_string(&pool->album_pool, data[ALBUM]);
  s->genre = intern_string(&pool->genre_pool, data[GENRE]);
}

void set_release_date(song *s, const char *field)
{
  sscanf(field, "%d-%c-%c", &s->release_date.year, &s->release_date.month,
         &s->release_date.day);
}

void set_metrics(song *s, char **data)
{
  s->duration = atof(data[DURATION]);
  s->popularity = (char)atoi(data[POPULARITY]);
  s->stream = atoi(data[STREAM]);
}

void set_language(song *s, const char *field, songPool *pool)
{
  s->language = intern_string(&pool->language_pool, field);
}

void set_misc(song *s, char **data, songPool *pool)
{
  s->explicit_content =
      strcmp(data[EXPLICIT_CONTENT], "Yes") == 0 ? True : False;
  s->label = intern_string(&pool->label_pool, data[LABEL]);
  s->composer = intern_string(&pool->composer_pool, data[COMPOSER]);
  s->producer = intern_string(&pool->producer_pool, data[PRODUCER]);
  s->collaboration =
      intern_string(&pool->collaboration_pool, data[COLLABORATION]);
}

song *build_song_from_fields(char **data, songPool *pool)
{
  song *s = malloc(sizeof(song));
  if (!s)
    return NULL;

  set_basic_info(s, data, pool);
  set_release_date(s, data[RELEASE_DATE]);
  set_metrics(s, data);
  set_language(s, data[LANGUAGE], pool);
  set_misc(s, data, pool);

  return s;
}

song **load_songs_from(char *filename, int *total, songPool *pool)
{
  FILE *archivo = fopen(filename, "r");
  if (!archivo)
  {
    perror("Error al abrir archivo");
    exit(EXIT_FAILURE);
  }

  int capacity = 10;
  int count = 0;
  song **songs = malloc(sizeof(song *) * capacity);
  if (!songs)
    return NULL;

  char linea[256];
  fgets(linea, sizeof(linea), archivo);

  while (fgets(linea, sizeof(linea), archivo))
  {
    int field_count = 15;
    char **fields = split(linea, &field_count, ',');
    if (field_count != 15)
    {
      fprintf(stderr, "Error: línea con campos incompletos.\n");
      continue;
    }

    if (count >= capacity)
    {
      capacity *= 2;
      songs = realloc(songs, sizeof(song *) * capacity);
    }

    song *s = build_song_from_fields(fields, pool);
    if (!s)
      continue;

    songs[count++] = s;
    free(fields);
  }
  songs = realloc(songs, sizeof(song *) * count);
  fclose(archivo);
  *total = count;
  return songs;
}

void song_free(song *s)
{
  if (!s)
    return;
  free(s->title);
  free(s);
}

// Para evitar error de llamada antes de declaración
void menu(songPool pool, song **songs);

void menu2(int n_canciones, song **filtered_songs, song **songs, songPool pool)
{
  int ans;
  printf("\n=== %d canciones encontradas ===\n"
         "1. Ver primeras 100 canciones \n"
         "2. Aplicar otro filtro\n"
         "3. Exportar y salir\n"
         "4. Salir sin exportar\n",
         n_canciones);

  scanf("%d", &ans);
  system("clear");

  switch (ans)
  {
  case 1:
    printf("Se muestran las primeras 100 canciones:\n");
    for (int i = 0; i < n_canciones && i < 100; i++)
    {
      printf("%3d. %s\n", i + 1, filtered_songs[i]->title);
    }
    break;

  case 2:
    printf("Se aplica otro filtro\n");
    menu(
        pool,
        songs); // ESTO NO VA A FUNCIONAR PORQUE NO SE ENTREGA LA LISTA FILTRADA
    break;

  case 3:
    printf("Se exporta y se sale\n"); // FALTA TERMINAR
    break;

  case 4:
    printf("Se sale sin exportar\n");
    exit(0);
    break;

  default:
    printf("Opcion no valida\n");
    menu2(n_canciones, filtered_songs, songs, pool);
    break;
  }
}

int filtro_artista(songPool pool, song **songs)
{
  printf("Primeros 100 Artistas disponibles:\n");

  // Se imprime el pool de artistas
  for (int i = 0; i < pool.artist_pool.filled && i < 100; i++)
  {
    if (pool.artist_pool.filled > 0)
    {
      printf("%3d. %s\n", i + 1, pool.artist_pool.strings[i]);
    }
    else
    {
      printf("No hay artistas en el pool\n");
      return 0;
    }
  }

  // int size = pool.artist_pool.filled;

  // Total canciones
  int total_songs = 0;
  while (songs[total_songs] != NULL)
  {
    total_songs++;
  }

  printf("Ingrese el número del artista a filtrar: ");
  int ans;
  scanf("%d", &ans);
  system("clear");
  printf("Se filtra por artista: %s\n", pool.artist_pool.strings[ans - 1]);

  // Canciones por artista seleccionado
  int n_canciones = 0;

  // Canciones filtradas
  song **filtered_songs = malloc(sizeof(song *) * total_songs);

  for (int i = 0; i < total_songs; i++)
  {
    if (strcmp(songs[i]->artist, pool.artist_pool.strings[ans - 1]) == 0)
    {
      filtered_songs[n_canciones] = songs[i];
      n_canciones++;
    }
  }

  menu2(n_canciones, filtered_songs, songs, pool);

  free(filtered_songs);
  return 0;
}

int filtro_album(songPool pool, song **songs)
{
  printf("Primeros 100 albumes disponibles:\n");

  // Se imprime el pool de albumes
  for (int i = 0; i < pool.album_pool.filled && i < 100; i++)
  {
    if (pool.album_pool.filled > 0)
    {
      printf("%3d. %s\n", i + 1, pool.album_pool.strings[i]);
    }
    else
    {
      printf("No hay albumes en el pool\n");
      return 0;
    }
  }

  // Total canciones
  int total_songs = 0;
  while (songs[total_songs] != NULL)
  {
    total_songs++;
  }

  printf("Ingrese el número del album a filtrar: ");
  int ans;
  scanf("%d", &ans);
  system("clear");
  printf("Se filtra por album: %s\n", pool.album_pool.strings[ans - 1]);

  // Canciones por artista seleccionado
  int n_canciones = 0;

  // Canciones filtradas
  song **filtered_songs = malloc(sizeof(song *) * total_songs);

  for (int i = 0; i < total_songs; i++)
  {
    if (strcmp(songs[i]->album, pool.album_pool.strings[ans - 1]) == 0)
    {
      filtered_songs[n_canciones] = songs[i];
      n_canciones++;
    }
  }

  menu2(n_canciones, filtered_songs, songs, pool);

  free(filtered_songs);
  return 0;
}

int filtro_genero(songPool pool, song **songs)
{
  printf("Primeros 100 generos disponibles:\n");

  // Se imprime el pool de albumes
  for (int i = 0; i < pool.genre_pool.filled && i < 100; i++)
  {
    if (pool.genre_pool.filled > 0)
    {
      printf("%3d. %s\n", i + 1, pool.genre_pool.strings[i]);
    }
    else
    {
      printf("No hay generos en el pool\n");
      return 0;
    }
  }

  // Total canciones
  int total_songs = 0;
  while (songs[total_songs] != NULL)
  {
    total_songs++;
  }

  printf("Ingrese el número del genero a filtrar: ");
  int ans;
  scanf("%d", &ans);
  system("clear");
  printf("Se filtra por genero: %s\n", pool.genre_pool.strings[ans - 1]);

  // Canciones por artista seleccionado
  int n_canciones = 0;

  // Canciones filtradas
  song **filtered_songs = malloc(sizeof(song *) * total_songs);

  for (int i = 0; i < total_songs; i++)
  {
    if (strcmp(songs[i]->genre, pool.genre_pool.strings[ans - 1]) == 0)
    {
      filtered_songs[n_canciones] = songs[i];
      n_canciones++;
    }
  }

  menu2(n_canciones, filtered_songs, songs, pool);

  free(filtered_songs);
  return 0;
}

int filtro_idioma(songPool pool, song **songs)
{
  printf("Primeros 100 idiomas disponibles:\n");

  for (int i = 0; i < pool.language_pool.filled && i < 100; i++)
  {
    if (pool.language_pool.filled > 0)
    {
      printf("%3d. %s\n", i + 1, pool.language_pool.strings[i]);
    }
    else
    {
      printf("No hay lenguajes en el pool\n");
      return 0;
    }
  }

  // Total canciones
  int total_songs = 0;
  while (songs[total_songs] != NULL)
  {
    total_songs++;
  }

  printf("Ingrese el número del lenguaje a filtrar: ");
  int ans;
  scanf("%d", &ans);
  system("clear");
  printf("Se filtra por lenguaje: %s\n", pool.language_pool.strings[ans - 1]);

  int n_canciones = 0;

  // Canciones filtradas
  song **filtered_songs = malloc(sizeof(song *) * total_songs);

  for (int i = 0; i < total_songs; i++)
  {
    if (strcmp(songs[i]->language, pool.language_pool.strings[ans - 1]) == 0)
    {
      filtered_songs[n_canciones] = songs[i];
      n_canciones++;
    }
  }

  menu2(n_canciones, filtered_songs, songs, pool);

  free(filtered_songs);
  return 0;
}

int filtro_ano()
{
  printf("Se filtra por año\n");
  return 0;
}

int filtro_duracion()
{
  printf("Se filtra por duracion\n");
  return 0;
}

int (*filtros[])(songPool, song **) = {filtro_artista, filtro_album,
                                       filtro_genero,  filtro_idioma,
                                       filtro_ano,     filtro_duracion};

void menu(songPool pool, song **songs)
{
  int ans;
  printf("\n===== SISTEMA DE FILTRADO DE CANCIONES =====\n\n"
         "Seleccione un filtro para aplicar:\n"
         "1. Filtrar por artista\n"
         "2. Filtrar por álbum\n"
         "3. Filtrar por género\n"
         "4. Filtrar por idioma\n"
         "5. Filtrar por año de lanzamiento (rango)\n"
         "6. Filtrar por duración (rango en segundos)\n"
         "7. Salir sin exportar\n\n");
  printf("Seleccione una opción: ");

  scanf("%d", &ans);
  system("clear");
  filtros[ans - 1](pool, songs);
}

int main(int argc, char **argv)
{
  if (argc == 1)
    show_usage_error(argv[0]);

  songPool pool;
  init_song_pool(&pool);

  int total_songs = 0;
  song **songs = load_songs_from(argv[1], &total_songs, &pool);

  menu(pool, songs);

  for (int i = 0; i < total_songs; i++)
  {
    // printf("%s\n", songs[i]->artist);
    song_free(songs[i]);
  }

  free(songs);
  free_song_pool(&pool);

  return 0;
}
