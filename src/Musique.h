#include <iostream>
#include <vector>
#include <string>   //to_string()

#include "json/json.h"

#ifndef MUSIQUE_H
#define MUSIQUE_H

enum class Protocole{
    DDL,
    torrent,
    magnet
};

struct Source{
    std::string lien;
    Protocole protocole;

    Source(std::string lien,Protocole protocole);
    ~Source();
};

class Telechargement
{
    Source source;
    public:
        Telechargement(Source source);
        ~Telechargement();

        void doIt();    //varie selon le protocole de la source
};

//faire format 2 digits par champs -> HH:MM:SS ou juste MM:SS si 0H
struct Duree{
    unsigned short h,m,s;

    Duree(unsigned short h=0, unsigned short m=0, unsigned short s=0);
    ~Duree();
    std::string getH()const;
    std::string getM()const;
    std::string getS()const;
    std::string get()const;
};

class Musique
{
    std::string nomAlbum,titre;
    std::vector<std::string>artistes;
    std::vector<Source>sources;
    unsigned short anneeAlbum;
    Duree duree;
    //ensemble de fichiers

    public:
        Musique();
        Musique(std::string titre,std::string nomAlbum,std::vector<std::string>artistes,unsigned short anneeAlbum,Duree duree,
            std::vector<Source>sources=std::vector<Source>());
        ~Musique();

        std::string getTitre()const;
        std::string getNomAlbum()const;
        unsigned short getAnneeAlbum()const;
        Duree getDuree()const;
        std::vector<std::string> getArtistes()const;
        std::vector<Source> getSources()const;

        void setTitre(std::string titre);
        void setNomAlbum(std::string album);
        void setAnneeAlbum(unsigned short annee);
        void setDuree(Duree duree);
        void setDuree(unsigned short h,unsigned short m,unsigned short s);
        void setArtistes(std::vector<std::string> artistes);
        void setSources(std::vector<Source>sources);

        static std::vector<Musique> jsonObjToObj(Json::Value value);
        static Json::Value objToJsonObj(std::vector<Musique>musiques);
        //on peut faire 2 maps const à la place des switchs
        static std::string protoToStr(Protocole protocole);
        static Protocole strToProto(std::string str);
};
#endif