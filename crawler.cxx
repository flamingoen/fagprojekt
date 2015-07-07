/*
 * cCrawler.cxx
 *
 *  Created on: 08/04/2015
 *      Authors: Martin Obel Thomsen, Daniel Blyme Gruntwig, Andreas Rømer Hjorth
 *
 */

#include <crawler.h>


QList<QStringList> getSeries(QString dirPath){ // s134862
    return sortSets(dirCrawler(dirPath));
}

// List the files in "dir_name".

QStringList dirCrawler(QString dirPath) { // s132106
    
    DIR * directory;
    QStringList filesList;
    
    // Open the directory specified by "dir_name".
    directory = opendir(dirPath.toStdString().c_str());
    
    // Check it was opened.
    if (!directory) {
        qDebug() << "directory could not be opened";
        exit(EXIT_FAILURE);
    }
    
    // main loop - recursivly crawls though files and stops when no more files is found
    while (1) {
        struct dirent * entry;
        std::string d_filename;
        QString full_path;
        std::string path;
        QStringList subList;
        
        // "Readdir" gets subsequent entries from "directory".
        // if their is no more entries the loop should break
        entry = readdir(directory);
        if (!entry) break;
        
        // gets filename from struct
        d_filename = entry->d_name;
        
        // check if the file is of FILETYPE and if true adds it to the list
        if(d_filename.substr(d_filename.find_last_of(".") + 1) == FILETYPE ||
            d_filename.substr(d_filename.find_last_of(".") + 1) == "vtu"
        ) {
            full_path = (dirPath.toStdString()+"/"+d_filename).c_str();
            filesList << full_path;
            //filesList << listItem;
        }
        
        if (entry->d_type & DT_DIR) { //TODO: magi sker her
            // Check that the directory is not itself or its parent.
            if (d_filename != ".." && d_filename != ".") {
                path = dirPath.toStdString()+"/"+d_filename;
                // Recursively call "dirCrawler" with the new path.
                subList = dirCrawler(path.c_str());
            }
        }
        
        for (int i=0 ; i<subList.size() ; i++) {
            filesList << subList[i];
        }
    }
    
    // closing the directory.
    if (closedir(directory)) {
        std::cout << "Failed to close file: " << directory << std::endl;
        exit(EXIT_FAILURE);
    }
    return filesList;
} 

QList<QStringList> sortSets(QStringList list) { // s134835
    QList<QStringList> setList;
    QString current, next;
    int index;
    
    list.sort();
    
    while (list.size()!=0){
        QStringList timeStepList;
        
        current = list.takeFirst();
        timeStepList << current;
        index = indexOfFistInt(current);
        
        if (index!=-1){
            next = list.takeFirst();
            
            while ( list.size()!=0 && 
                !QString::compare(current.left(index-1),next.left(index-1), 
                                 Qt::CaseInsensitive)) {
                timeStepList << next;
                next = list.takeFirst();
            }
        }
        
        setList << timeStepList;
    }
    
    return setList;
}

// returns the first index of the first integer in the string. 
// uses the regex [0..9]{1,} meaning 0 to 9 at least once
int indexOfFistInt(QString str) { // s134862
    return str.indexOf(QRegExp("[0-9]{4,}"));
}
