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

char *strdup(const char *s) {
  size_t len = strlen(s) + 1;
  char *copy = malloc(len);
  if (copy) strcpy(copy, s);
  return copy;
}

void remove_newline(char *linea) {
  size_t len = strlen(linea);
  if (len > 0 && linea[len - 1] == '\n') linea[len - 1] = '\0';
}

int count_delimiters(const char *linea, char separator) {
  int count = 1;
  for (const char *c = linea; *c; c++)
    if (*c == separator) count++;
  return count;
}

char *create_field(const char *start, const char *end) {
  size_t len = end - start;
  char *field = malloc(len + 1);
  if (!field) return NULL;
  strncpy(field, start, len);
  field[len] = '\0';
  return field;
}

char **split(const char *linea_original, int *count, char separator) {
  char *linea = strdup(linea_original);
  if (!linea) return NULL;

  remove_newline(linea);
  int n = count_delimiters(linea, separator);
  int i = 0;

  char **result = malloc(sizeof(char *) * n);
  if (!result) {
    free(linea);
    return NULL;
  }

  const char *start = linea;
  for (const char *ptr = linea;; ptr++) {
    if (*ptr == separator || *ptr == '\0') {
      result[i++] = (ptr == start) ? strdup("") : create_field(start, ptr);
      if (*ptr == '\0') break;
      start = ptr + 1;
    }
  }

  *count = n;
  free(linea);
  return result;
}

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
