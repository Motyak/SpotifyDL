#include "Musique.h"

Telechargement::Telechargement(Source source) : source(source)
{
    ;
}

Telechargement::~Telechargement()
{
    ;
}

void Telechargement::doIt()    //varie selon le protocole de la source
{
    if(this->source.protocole==Protocole::magnet)
        ;
        
    else (this->source.protocole==Protocole::DDL)
        ;
}

Source::Source(std::string lien,Protocole protocole) : lien(lien),protocole(protocole)
{
    ;
}

Source::~Source()
{
    ;
}

Duree::Duree(unsigned short h, unsigned short m, unsigned short s) : h(h),m(m),s(s)
{
    ;
}

Duree::~Duree()
{
    ;
}

std::string Duree::getH()const
{
    if(h<10)
        return "0"+std::to_string(h);
    else
        return std::to_string(h);
}

std::string Duree::getM()const
{
    if(m<10)
        return "0"+std::to_string(m);
    else
        return std::to_string(m);
}

std::string Duree::getS()const
{
    if(s<10)
        return "0"+std::to_string(s);
    else
        return std::to_string(s);
}

std::string Duree::get()const
{
    if(h==0)
        return getM()+":"+getS();
    else
        return getH()+":"+getM()+":"+getS();
}

Musique::Musique() : titre(""),nomAlbum(""),artistes({}),anneeAlbum(0),duree()
{
    ;
}

//Musique musique("Paperplanes","JeSaisPasTropTrop",std::vector<std::string>{"M.I.A"},2007,Duree(0,3,25));
Musique::Musique(std::string titre,std::string nomAlbum,std::vector<std::string>artistes,unsigned short anneeAlbum,Duree duree,
    std::vector<Source>sources) : titre(titre),nomAlbum(nomAlbum),artistes(artistes),anneeAlbum(anneeAlbum),duree(duree),sources(sources)
{
    ;
}

Musique::~Musique()
{
    ;
}

std::string Musique::getTitre()const
{
    return this->titre;
}

std::string Musique::getNomAlbum()const
{
    return this->nomAlbum;
}

unsigned short Musique::getAnneeAlbum()const
{
    return this->anneeAlbum;
}

Duree Musique::getDuree()const
{
    return this->duree;
}

std::vector<std::string> Musique::getArtistes()const
{
    return this->artistes;
}

std::vector<Source> Musique::getSources()const
{
    return this->sources;
}

void Musique::setTitre(std::string titre)
{
    this->titre=titre;
}

void Musique::setNomAlbum(std::string album)
{
    this->nomAlbum=album;
}

void Musique::setAnneeAlbum(unsigned short annee)
{
    this->anneeAlbum=annee;
}

void Musique::setDuree(Duree duree)
{
    this->duree=duree;
}

void Musique::setDuree(unsigned short h,unsigned short m,unsigned short s)
{
    this->duree.h=h;
    this->duree.m=m;
    this->duree.s=s;
}

void Musique::setArtistes(std::vector<std::string> artistes)
{
    this->artistes=artistes;
}

void Musique::setSources(std::vector<Source> sources)
{
    this->sources=sources;
}

std::vector<Musique> Musique::jsonObjToObj(Json::Value value)
{
    std::vector<Musique>playlist;
    for(unsigned short index=0;index<value.size();++index)
    {
        Musique mus;
        mus.setTitre(value[index]["titre"].asString());
        mus.setNomAlbum(value[index]["nomAlbum"].asString());
        mus.setAnneeAlbum(value[index]["anneeAlbum"].asInt());
        mus.setDuree(0,value[index]["duree"]["minutes"].asInt(),value[index]["duree"]["seconds"].asInt());
        //mus.getDuree().m=value[index]["duree"]["minutes"].asInt();
        //mus.getDuree().s=value[index]["duree"]["seconds"].asInt();
        for(unsigned short i=0;i<value[index]["artistes"].size();++i)
        {
            mus.getArtistes().push_back(value[index]["artistes"][i].asString());
        }
        playlist.push_back(mus);
    }
    return playlist;
}

Json::Value Musique::objToJsonObj(std::vector<Musique> musiques)
{
    Json::Value playlist;
    for(unsigned short index=0;index<musiques.size();++index)
    {
        Json::Value val;
        val["titre"]=musiques[index].getTitre();
        val["nomAlbum"]=musiques[index].getNomAlbum();
        val["anneeAlbum"]=musiques[index].getAnneeAlbum();
        //val["duree"]["hours"]=musiques[index].duree.h;
        val["duree"]["minutes"]=musiques[index].getDuree().m;
        val["duree"]["seconds"]=musiques[index].getDuree().s;
        for(unsigned short i=0;i<musiques[index].getArtistes().size();++i)
        {
            Json::Value v(musiques[index].getArtistes()[i]);
            val["artistes"].append(v);
        }
        for(unsigned short i=0;i<musiques[index].getSources().size();++i)
        {
            Json::Value lien(musiques[index].getSources()[i].lien);
            Json::Value protocole(Musique::protoToStr(musiques[index].getSources()[i].protocole));
            Json::Value v;
            v["lien"]=lien;
            v["protocole"]=protocole;
            val["sources"].append(v);
        }

        playlist.append(val);
    }

    return playlist;
}

std::string Musique::protoToStr(Protocole protocole)
{
    if(protocole==Protocole::magnet)
        return "magnet";
    else if(protocole==Protocole::DDL)
        return "DDL";
    else    //protocole==Protocole::torrent
        return "torrent";
}

Protocole Musique::strToProto(std::string str)
{
    if(str=="magnet")
        return Protocole::magnet;
    else if(str=="DDL")
        return Protocole::DDL;
    else    //str=="torrent"
        return Protocole::torrent;
}

#ifndef NDEBUG
#include <cassert>
#ifdef _MUSIQUE_UT_
//g++ -o ../bin/Musique_UT Musique.cpp -I . -ljsoncpp -D _MUSIQUE_UT_
int main()
{
    Musique msq("titre","nomAlbum",{"artiste"},2018,Duree(0,3,0));
    std::vector<Musique>vec{msq};
    Json::Value v=Musique::objToJsonObj(vec);
    std::cout<<v<<"\n";
    std::vector<Musique>vec2=Musique::jsonObjToObj(v);
    for(unsigned int i=0;i<vec2.size();++i)
        std::cout<<"duree : "<<vec.at(i).getDuree().get()<<"\ntitre : "<<vec.at(i).getTitre()<<"\n";
    

    return 0;
}
#endif
#endif