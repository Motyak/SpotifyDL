#include "md5/md5.h"

#include <iostream>
#include <fstream>
#include <experimental/filesystem>   //file_size() ; -lstdc++fs
#include <algorithm>                 //reverse()

#ifndef FICHIER_H
#define FICHIER_H

//classe pour enregistrer le chemin d'un fstream
class Fstream : public std::fstream
{
    std::string path;
    public:
        Fstream(std::string path);
        ~Fstream();
        std::string getPath()const;
};


class Fichier
{
    std::string md5;
    unsigned int taille;
    std::string nom;

    public:
        Fichier();
        Fichier(std::string md5,unsigned int taille,std::string nom);
        virtual ~Fichier();

        static Fichier fstreamToFichier(Fstream& fstr);

        std::string getMd5()const;
        unsigned int getTaille()const;
        std::string getNom()const;
        void setMd5(std::string md5);
        void setTaille(unsigned int taille);
        void setNom(std::string nom);

        bool is_same(Fichier fichier);
};
#endif