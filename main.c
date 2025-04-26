#include <stdio.h>
#include <stdlib.h>

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

typedef struct {
  int year;
  char month;
  char day;
} Date;

typedef struct {
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

typedef struct {
  song *songs;
  int size;
  int capacity;
} SongList;

/* ===UTILITARIOS=== */
void show_usage_error(const char *__name__) {
  fprintf(stderr, "Error: You must provide the CSV file as an argument.\n");
  fprintf(stderr, "Usage: %s <filename.csv>\n", __name__);
  exit(EXIT_FAILURE);
}

/* song Song_create(char *id, char *title, char *artist, char *album, char
*genre, char *release_date, int duration, int popularity, int stream, char
*language, char *explicit_content, char *label, char *composer, char *producer,
char *collaboration) { song s = { id,       title,        artist, album, genre,
release_date, duration,         popularity, stream,   language,
explicit_content, label, composer, producer,     collaboration,
  };

  return s;
} */

/* void SongList_free(SongList *list) {
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
} */
/* ===Cargar Canciones=== */
SongList *SongList_create(int capacity) {
  SongList *list = malloc(sizeof(SongList));
  list->songs = malloc(sizeof(song) * capacity);
  list->size = 0;
  list->capacity = capacity;
  return list;
}

void SongList_add(SongList *list, song song) {
  if (list->size == list->capacity) {
    list->capacity *= 2;
    list->songs = realloc(list->songs, sizeof(song) * list->capacity);
  }
  list->songs[list->size++] = song;
}

/* void SongList_load_from_csv(SongList *list, const char *filename) {
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

    id = id ? strdup(id) : strdup("None");
    title = title ? strdup(title) : strdup("None");
    artist = artist ? strdup(artist) : strdup("None");
    album = album ? strdup(album) : strdup("None");
    genre = genre ? strdup(genre) : strdup("None");
    release_date = release_date ? strdup(release_date) : strdup("None");
    language = language ? strdup(language) : strdup("None");
    explicit_content =
        explicit_content ? strdup(explicit_content) : strdup("None");
    label = label ? strdup(label) : strdup("None");
    composer = composer ? strdup(composer) : strdup("None");
    producer = producer ? strdup(producer) : strdup("None");
    collaboration = collaboration ? strdup(collaboration) : strdup("None");

    int duration = duration_str ? atoi(duration_str) : 0;
    int popularity = popularity_str ? atoi(popularity_str) : 0;
    int stream = stream_str ? atoi(stream_str) : 0;

    song song =
        Song_create(id, title, artist, album, genre, release_date, duration,
                    popularity, stream, language, explicit_content, label,
                    composer, producer, collaboration);

    SongList_add(list, song);
  }

  fclose(f);
} */

int main(int argc, char **argv) {
  if (argc == 1) show_usage_error(argv[0]);

  return 0;
}
