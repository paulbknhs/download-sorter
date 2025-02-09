#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define DOWNLOADS_DIR "/home/paul/Downloads/"

int index_of_last_dot(char *filename) {
  int index = -1;
  for (int i = 0; filename[i]; i++) {
    if (filename[i] == '.')
      index = i;
  }
  return index;
}

int main() {
  DIR *dir = opendir(DOWNLOADS_DIR);
  if (!dir) {
    perror("Unable to open directory");
    return 1;
  }

  struct dirent *entry;
  while ((entry = readdir(dir)) != NULL) {
    if (entry->d_type == DT_REG) {
      char *filename = entry->d_name;
      int dot_index = index_of_last_dot(filename);

      if (dot_index > 0 && dot_index < (int)strlen(filename) - 1) {
        char *extension = filename + dot_index + 1;

        // Erstelle Ordnerpfad
        char new_dir_path[PATH_MAX];
        snprintf(new_dir_path, sizeof(new_dir_path), "%s/%s", DOWNLOADS_DIR,
                 extension);

        // Erstelle Ordner falls nicht vorhanden
        struct stat st = {0};
        if (stat(new_dir_path, &st) == -1) {
          mkdir(new_dir_path, 0700);
        }

        // Verschiebe Datei
        char old_path[PATH_MAX];
        char new_path[PATH_MAX];
        snprintf(old_path, sizeof(old_path), "%s/%s", DOWNLOADS_DIR, filename);
        snprintf(new_path, sizeof(new_path), "%s/%s", new_dir_path, filename);

        if (rename(old_path, new_path) != 0) {
          perror("Error moving file");
        }
      }
    }
  }

  closedir(dir);
  return 0;
}
