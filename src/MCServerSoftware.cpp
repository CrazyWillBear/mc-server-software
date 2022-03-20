// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <cstring>
#include <iostream>
#include <string>
#include <unistd.h>
#include "config.h"

int main(int argc, char *argv[]) {
    double fullBackupCountReq = (worldBackupInterval * 60) / (fullBackupInterval * 60 * 60);
    int fullBackupCount = 0;

    std::cout << "::Full server updates will occur every " << fullBackupCountReq << " cycles";

    while (true) {
        std::cout << "\n\n::Sleeping for " << worldBackupInterval * 60 << " seconds\n";

        // sleep minutes requested
        sleep(worldBackupInterval * 60);
        fullBackupCount++;

        // get date
        char buffer[128];
        std::string date = "";
        FILE* pipe = popen("date +\"%m-%e-%y--%I.%M.%S\"" ,"r");
        if (!pipe) throw std::runtime_error("popen() failed!");
        try {
            while (fgets(buffer, sizeof buffer, pipe) != NULL) { date += buffer; }
        } catch (...) {
            pclose(pipe);
            throw;
        }
        pclose(pipe);

        date.pop_back();

        // check excess saves
        std::string result = "";
        pipe = popen("find ./backups -mindepth 1 -maxdepth 11 -type d | wc -l", "r");
        if (!pipe) throw std::runtime_error("popen() failed!");
        try {
            while (fgets(buffer, sizeof buffer, pipe) != NULL) { result += buffer; }
        } catch (...) {
            pclose(pipe);
            throw;
        }
        pclose(pipe);

        if (std::stoi(result) >= 10) {
            std::cout << "::Deleting oldest backup...\n";
            std::system("rm -rf ./backups/$(ls -t ./backups | tail -1)");
        }

        // make backup directory
        std::string cmd = "mkdir -p ./backups/" + date;
        std::system(cmd.c_str());

        // backup overworld
        std::cout << "\n\n::Adding ./world to ./backups/" << date << "/world.tar.gz\n";
        cmd = "tar -czvf ./backups/" + date + "/world.tar.gz ./world";
        std::system(cmd.c_str());

        // backup nether
        std::cout << "::\n\nAdding ./world_nether to ./backups/" << date << "/world_nether.tar.gz\n";
        cmd = "tar -czvf ./backups/" + date + "/world_nether.tar.gz ./world_nether";
        std::system(cmd.c_str());

        // backup end
        std::cout << "::\n\nAdding ./world_end to ./backups/" << date << "/world_end.tar.gz\n";
        cmd = "tar -czf ./backups/" + date + "/world_end.tar.gz ./world_end";
        std::system(cmd.c_str());

        // check for full backup and do if required
        if (fullBackupCount >= fullBackupCountReq && fullBackupsEnabled) {
            std::cout << "::Commencing full server backup...\n";
            cmd = "git add .; git commit -m \"" + date + "\"; git push";
            std::system(cmd.c_str());

            fullBackupCount = 0;
        }
    }
    return 0;
}