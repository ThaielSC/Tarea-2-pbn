#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// INITIAL ERROR HANDLE
void show_usage_error(const char *__name__)
{
  fprintf(stderr, "Error: You must provide the CSV file as an argument.\n");
  fprintf(stderr, "Usage: %s <filename.csv>\n", __name__);
  exit(EXIT_FAILURE);
}

typedef struct song Song;

struct song
{
  char *id;
  char *title;
  char *artist;
  char *album;
  char *genre;
  char *release_date;
  int duration;
  int popularity;
  int stream;
  char *language;
  char *explicit_content;
  char *label;
  char *composer;
  char *producer;
  char *collaboration;
  void (*show)(const Song *self);
};

void song_show(const Song *song)
{
  printf("\nInformacion de canción:\n");
  printf("Title: %s; ", song->title);
  printf("Artist: %s; ", song->artist);
  printf("Album: %s; ", song->album);
  printf("Genre: %s; ", song->genre);
  printf("Release Date: %s; ", song->release_date);
  printf("Duration: %d seconds; ", song->duration);
  printf("Popularity: %d; ", song->popularity);
  printf("Streams: %d; ", song->stream);
  printf("Language: %s; ", song->language);
  printf("Explicit Content: %s; ", song->explicit_content);
  printf("Label: %s; ", song->label);
  printf("Composer: %s; ", song->composer);
  printf("Producer: %s; ", song->producer);
  printf("Collaboration: %s\n", song->collaboration);
}

Song Song_create(char *id, char *title, char *artist, char *album, char *genre,
                 char *release_date, int duration, int popularity, int stream,
                 char *language, char *explicit_content, char *label,
                 char *composer, char *producer, char *collaboration)
{
  Song s = {id, title, artist, album,
            genre, release_date, duration, popularity,
            stream, language, explicit_content, label,
            composer, producer, collaboration, song_show};

  return s;
}

// CREAR LISTA DE CANCIONES
typedef struct SongList
{
  Song *songs;
  int size;
  int capacity;
} SongList;

SongList *SongList_create(int capacity)
{
  SongList *list = malloc(sizeof(SongList));
  list->songs = malloc(sizeof(Song) * capacity);
  list->size = 0;
  list->capacity = capacity;
  return list;
}

// AGREGAR CANCIONES A LA LISTA
void SongList_add(SongList *list, Song song)
{
  if (list->size == list->capacity)
  {
    list->capacity *= 2;
    list->songs = realloc(list->songs, sizeof(Song) * list->capacity);
  }
  list->songs[list->size++] = song;
}

// LIBERAR MEMORIA
void SongList_free(SongList *list)
{

  for (int i = 0; i < list->size; i++) {
    free(list->songs[i].id);
    free(list->songs[i].title);
    free(list->songs[i].artist);
    free(list->songs[i].album);
    free(list->songs[i].genre);
    free(list->songs[i].release_date);
    free(list->songs[i].language);
    free(list->songs[i].explicit_content);
    free(list->songs[i].label);
    free(list->songs[i].composer);
    free(list->songs[i].producer);
    free(list->songs[i].collaboration);
  }

  free(list->songs);
  free(list);
}

// LEER ARCHIVO CSV Y CARGAR CANCIONES
void SongList_load_from_csv(SongList *list, const char *filename) {
  FILE *f = fopen(filename, "r");

  char line[1000];
  int is_header = 1;

  while (fgets(line, sizeof(line), f)) {
    // PARA NO LEER LA PRIMERA LÍNEA
    if (is_header) {
      is_header = 0;
      continue;
    }

    char *id = strtok(line, ",");
    char *title = strtok(NULL, ",");
    char *artist = strtok(NULL, ",");
    char *album = strtok(NULL, ",");
    char *genre = strtok(NULL, ",");
    char *release_date = strtok(NULL, ",");
    char *duration_str = strtok(NULL, ",");
    char *popularity_str = strtok(NULL, ",");
    char *stream_str = strtok(NULL, ",");
    char *language = strtok(NULL, ",");
    char *explicit_content = strtok(NULL, ",");
    char *label = strtok(NULL, ",");
    char *composer = strtok(NULL, ",");
    char *producer = strtok(NULL, ",");
    char *collaboration = strtok(NULL, ",");

    // ASIGNAR MEMORIA PARA LOS CHAR
    id = id ? strdup(id) : strdup("None");
    title = title ? strdup(title) : strdup("None");
    artist = artist ? strdup(artist) : strdup("None");
    album = album ? strdup(album) : strdup("None");
    genre = genre ? strdup(genre) : strdup("None");
    release_date = release_date ? strdup(release_date) : strdup("None");
    language = language ? strdup(language) : strdup("None");
    explicit_content = explicit_content ? strdup(explicit_content) : strdup("None");
    label = label ? strdup(label) : strdup("None");
    composer = composer ? strdup(composer) : strdup("None");
    producer = producer ? strdup(producer) : strdup("None");
    collaboration = collaboration ? strdup(collaboration) : strdup("None");

    // CONVERTIR DATOS A NUMEROS
    int duration = duration_str ? atoi(duration_str) : 0;
    int popularity = popularity_str ? atoi(popularity_str) : 0;
    int stream = stream_str ? atoi(stream_str) : 0;


    Song song = Song_create(id, title, artist, album, genre, release_date,
                            duration, popularity, stream, language,
                            explicit_content, label, composer, producer,
                            collaboration);

    SongList_add(list, song);
  }

  fclose(f);
}



int main(int argc, char **argv)
{
  if (argc == 1)
    show_usage_error(argv[0]);

  printf("DataBase: %s\n", argv[1]);

  Song mySong =
      Song_create("1", "My New Song", "Some Artist", "Some Album", "Pop",
                  "2025-04-23", 210, 75, 1000000, "EN", "No", "MyLabel",
                  "Composer Name", "Producer Name", "Feat. Guest");

  mySong.show(&mySong);

  SongList *list = SongList_create(100);

  SongList_load_from_csv(list, argv[1]);
  printf("Canciones guardadas: %d\n", list->size);

  // MOSTRAR CANCIONES
  for (int i = 0; i < list->size; i++)
  {
    list->songs[i].show(&list->songs[i]);
  }

  SongList_free(list);

  return 0;
}
