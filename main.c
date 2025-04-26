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
  ISO language;
  bool explicit_content;
  char *label;
  char *composer;
  char *producer;
  char *collaboration;
} song;

typedef struct
{
  song *songs;
  int size;
  int capacity;
} SongList;

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
      result[i++] = (ptr == start) ? strdup("") : create_field(start, ptr);
      if (*ptr == '\0')
        break;
      start = ptr + 1;
    }
  }

  *count = n;
  free(linea);
  return result;
}

/* ===Cargar Canciones=== */
void set_basic_info(song *s, char **data)
{
  strncpy(s->id, data[SONG_ID], 7);
  s->id[7] = '\0';

  s->title = data[SONG_TITLE];
  s->artist = data[ARTIST];
  s->album = data[ALBUM];
  s->genre = data[GENRE];
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

void set_language(song *s, const char *field)
{
  strncpy(s->language, field, 2);
  s->language[2] = '\0';
}

void set_misc(song *s, char **data)
{
  s->explicit_content =
      strcmp(data[EXPLICIT_CONTENT], "True") == 0 ? True : False;
  s->label = data[LABEL];
  s->composer = data[COMPOSER];
  s->producer = data[PRODUCER];
  s->collaboration = data[COLLABORATION];
}

song *build_song_from_fields(char **data)
{
  song *s = malloc(sizeof(song));
  if (!s)
    return NULL;

  set_basic_info(s, data);
  set_release_date(s, data[RELEASE_DATE]);
  set_metrics(s, data);
  set_language(s, data[LANGUAGE]);
  set_misc(s, data);

  return s;
}

song **load_songs_from(char *filename, int *total)
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

    song *s = build_song_from_fields(fields);
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
  free(s->artist);
  free(s->album);
  free(s->genre);
  free(s->label);
  free(s->composer);
  free(s->producer);
  free(s->collaboration);
  free(s);
}

void SongList_free(SongList *list)
{
  for (int i = 0; i < list->size; i++)
    song_free(list->songs);
  free(list->songs);
  free(list);
}

int main(int argc, char **argv)
{
  if (argc == 1)
    show_usage_error(argv[0]);

  int total_songs = 0;
  song **songs = load_songs_from(argv[1], &total_songs);

  printf("Se cargaron %d canciones.\n", total_songs);

  for (int i = 0; i < total_songs; i++)
  {
    printf("Título: %s\n", songs[i]->title);
    song_free(songs[i]);
  }
  free(songs);

  return 0;
}
