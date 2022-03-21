// Define each parameter according to how you would like
// the program to run. Don't forget to run the program
// in the same directory as your server!

#ifndef CONFIG_H
#define CONFIG_H

// how often to backup world files (minutes)
double worldBackupInterval = 60;

// enable full server backups (server must be a git repo)
bool fullBackupsEnabled = true;

// how often to backup all files (hours)
double fullBackupInterval = 6;

// maximum number of backups
int maxSaves = 6;

#endif
