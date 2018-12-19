#include "Fichier.h"

Fstream::Fstream(std::string path) : std::fstream(path,std::ios::binary),path(path)
{
    ;
}

Fstream::~Fstream()
{
    ;
}

std::string Fstream::getPath()const
{
    return this->path;
}

Fichier::Fichier() : md5(""),taille(0),nom("")
{
    ;
}

Fichier::Fichier(std::string md5,unsigned int taille,std::string nom) : md5(md5),taille(taille),nom(nom)
{
    ;
}

Fichier::~Fichier()
{
    ;
}

std::string Fichier::getMd5()const
{
    return this->md5;
}

unsigned int Fichier::getTaille()const
{
    return this->taille;
}

std::string Fichier::getNom()const
{
    return this->nom;
}

void Fichier::setMd5(std::string md5)
{
    this->md5=md5;
}

void Fichier::setTaille(unsigned int taille)
{
    this->taille=taille;
}

void Fichier::setNom(std::string nom)
{
    this->nom=nom;
}

bool Fichier::is_same(Fichier fichier)
{
    if(this->md5==fichier.md5&&this->taille==fichier.taille)
        return true;
    return false;
}

Fichier Fichier::fstreamToFichier(Fstream& fstr)
{
    Fichier fichier;

    //on extrait le nom du fichier à partir du chemin du fichier
    std::string path=fstr.getPath();
    std::reverse(path.begin(),path.end());
    unsigned short i=0;
    while(path[i]!='\0'&&path[i]!='/')
        i++;
    std::reverse(path.begin(),path.end());
    std::string nom=path.substr(path.size()-i,i);
    fichier.setNom(nom);

    fichier.setTaille(std::experimental::filesystem::file_size(fstr.getPath()));

    MD5 md5;
    std::ifstream ifs(fstr.getPath(),std::ios::binary);
    std::ostringstream oss;
    oss << ifs.rdbuf();
    std::string s(oss.str());
    md5.update(s.data(),s.size());
    md5.finalize();
    fichier.setMd5(md5.hexdigest());

    return fichier;
}

#ifndef NDEBUG
#include <cassert>
#ifdef _FICHIER_UT_
//g++ -o ../bin/Fichier_UT md5/md5.cpp Fichier.cpp -I . -I md5 -lstdc++fs -D _FICHIER_UT_
int main()
{
    Fstream fstr("test.flac");  //taille path 17;i=16;i=8 a la fin;taille nom 9
    Fstream msq("ruleTheWorld.flac");
    Fichier fichier=Fichier::fstreamToFichier(fstr);
    Fichier fichier2=Fichier::fstreamToFichier(msq);

    assert(fichier2.is_same(fichier));

    std::cout<<fichier.getNom()<<"\n";
    

    return 0;
}
#endif
#endif