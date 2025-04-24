#include <stdio.h>
#include <stdlib.h>

// INITIAL ERROR HANDLE
void show_usage_error(const char *__name__) {
  fprintf(stderr, "Error: You must provide the CSV file as an argument.\n");
  fprintf(stderr, "Usage: %s <filename.csv>\n", __name__);
  exit(EXIT_FAILURE);
}

typedef struct song Song;

struct song {
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

void song_show(const Song *song) {
  printf("Information de canción:\n");
  printf("Title: %s\n", song->title);
  printf("Artist: %s\n", song->artist);
  printf("Album: %s\n", song->album);
  printf("Genre: %s\n", song->genre);
  printf("Release Date: %s\n", song->release_date);
  printf("Duration: %d seconds\n", song->duration);
  printf("Popularity: %d\n", song->popularity);
  printf("Streams: %d\n", song->stream);
  printf("Language: %s\n", song->language);
  printf("Explicit Content: %s\n", song->explicit_content);
  printf("Label: %s\n", song->label);
  printf("Composer: %s\n", song->composer);
  printf("Producer: %s\n", song->producer);
  printf("Collaboration: %s\n", song->collaboration);
}

Song Song_create(char *id, char *title, char *artist, char *album, char *genre,
                 char *release_date, int duration, int popularity, int stream,
                 char *language, char *explicit_content, char *label,
                 char *composer, char *producer, char *collaboration) {
  Song s = {id,       title,        artist,           album,
            genre,    release_date, duration,         popularity,
            stream,   language,     explicit_content, label,
            composer, producer,     collaboration,    song_show};

  return s;
}

int main(int argc, char **argv) {
  if (argc == 1) show_usage_error(argv[0]);

  printf("DataBase: %s\n", argv[1]);

  Song mySong =
      Song_create("1", "My New Song", "Some Artist", "Some Album", "Pop",
                  "2025-04-23", 210, 75, 1000000, "EN", "No", "MyLabel",
                  "Composer Name", "Producer Name", "Feat. Guest");

  mySong.show(&mySong);

  return 0;
}
