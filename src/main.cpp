#include "Musique.h"
#include "Fichier.h"

//libs installées sur WSL, donc pas reconnu ici
//sudo apt-get install pkg-config libcurlpp-dev libcurl4-openssl-dev
#include <curlpp/cURLpp.hpp>    //-lcurlpp -lcurl
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>

//sudo apt-get install libjsoncpp-dev
#include "json/json.h"  //-ljsoncpp

#include <sstream>  //istreams
#include <fstream>  //fichiers
#include <string>   //substr
#include <cmath>   //round -> arrondir à la seconde près plutot que de tronquer
#include <iostream>

//g++ -o ../bin/main md5/md5.cpp Musique.cpp Fichier.cpp main.cpp -I . -I md5 -lcurlpp -lcurl -ljsoncpp -lstdc++fs
//-std=c++17 recommandé a cause de filesystem apparemment?
int main()
{
    /*Envoyer une requete pour recuperer le json de la playlist Spotify, puis je l'enregistre dans un fichier json*/

    //mettre le token en argument du prog ?
    const std::string OAUTH_TOKEN="BQB7ZHFgb0r59hfCj03HIYJ_rZbtQIg1I30qI7k-y7eFBcRzO3zCP8Vh3fPpjKcCP10LjwLXpmnizuLkb8rQtr494oMy9emAP9iobkisl7bXXVWXk4GbrLDNJJCFD2KuDCm1AREecPZM_g";
    const std::string PLAYLIST_ID="4YEwaHa5uYARV44afNEbNl";	//playlist radio bonheur
	const std::string JSON_PATH="../ressources/playlist.json";

    curlpp::Easy req;
    std::list<std::string> header;

    //std::fstream file("playlist.json",std::ios::out | std::ios::trunc);
    std::fstream file;

    header.push_back("Accept: application/json");
    header.push_back("Content-Type: application/json");
    header.push_back("Authorization: Bearer "+OAUTH_TOKEN);

    req.setOpt<curlpp::options::Url>("https://api.spotify.com/v1/playlists/"+PLAYLIST_ID);
    req.setOpt(curlpp::options::HttpHeader(header));

    file.open(JSON_PATH,std::ios::out | std::ios::trunc);
    
    req.perform();
    file << req;

    file.close();

///////////////////////////////////////////////////////

    /*je convertis le fichier json en une var Json::Value pour pouvoir l'utiliser*/

    //std::cout<<"\n\n\n\n";

    std::ifstream ss(JSON_PATH,std::ios::binary);//
 
    Json::Value root;

    ss >> root;

/////////////////////////////////////////////////////////

/*Je créé l'objet musiques à partir de l'objet JSON (je ne recupere que les champs qui m'interessent*/

std::vector<Musique> musiques;

for(unsigned short i=0;i<root["tracks"]["items"].size();++i)
{
    std::vector<std::string>artistes;
    for(unsigned short j=0;j<root["tracks"]["items"][i]["track"]["artists"].size();++j)
        artistes.push_back(root["tracks"]["items"][i]["track"]["artists"][j]["name"].asString());

    //il nous faut les 4 premiers caractères de release_date, converti en unsigned short
	//parfois il n'y a pas de champ release_date pour l'album
	unsigned short anneeAlbum;//
	try{//
    //unsigned short anneeAlbum=(unsigned short)std::stoi(root["tracks"]["items"][i]["track"]["album"]["release_date"].asString().substr(0,4));
	anneeAlbum=(unsigned short)std::stoi(root["tracks"]["items"][i]["track"]["album"]["release_date"].asString().substr(0,4));}//
	catch(...){}//

    unsigned int dureeEnMs=root["tracks"]["items"][i]["track"]["duration_ms"].asInt();
    //unsigned short nbH=dureeEnMs/3600000;
    //dureeEnMs-=nbH*3600000;
    unsigned short nbM=dureeEnMs/60000;
    dureeEnMs-=nbM*60000;
    //arrondir à la seconde la plus proche (pas forcemment la partie entiere donc)->round sur un float
    unsigned short nbS=round(dureeEnMs/1000.0f);
    //Duree duree((unsigned short)nbH,(unsigned short)nbM,(unsigned short)(nbS));
    Duree duree((unsigned short)0,(unsigned short)nbM,(unsigned short)(nbS));

    musiques.push_back(
    Musique(
        root["tracks"]["items"][i]["track"]["name"].asString(),
        root["tracks"]["items"][i]["track"]["album"]["name"].asString(),
        artistes,
        anneeAlbum,
        duree));
}
    //afficher durée de la 45eme musique de la playlist et le nom des artistes(i=numMusique+1)
   
   /*std::cout<<musiques[46].duree.get()<<std::endl;
   for(unsigned short index=0;index<musiques[46].artistes.size();++index)
   {
       std::cout<<musiques[46].artistes[index];
       if(index!=musiques[46].artistes.size()-1)
            std::cout<<",";
   }
   std::cout<<std::endl;*/

    //convertir l'objet musiques en une valeur JSON, puis l'enregistrer dans un fichier

    Json::Value objJson=Musique::objToJsonObj(musiques);
    file.open(JSON_PATH,std::ios::out | std::ios::trunc);
    file << objJson;
    file.close();

    //lire le fichier json puis convertir l'objet json en objet musiques

    /*std::ifstream stream("playlist.json",std::ios::binary);//
 
    Json::Value playlist;

    stream >> playlist;

    std::vector<Musique>liste=Musique::jsonObjToObj(playlist);

    std::cout<<"\n\n"<<liste[46].duree.get()<<"\n"<<liste[46].artistes[0]<<","<<liste[46].artistes[1]<<","<<liste[46].artistes[2]<<";"<<liste[46].titre<<std::endl;*/

    Fstream monFichier1("../ressources/ruleTheWorld.mp3");
    Fstream monFichier2("../ressources/test.mp3");
    Fichier fichier1;
    Fichier fichier2;
    fichier1=Fichier::fstreamToFichier(monFichier1);
    fichier2=Fichier::fstreamToFichier(monFichier2);
    std::cout<<"\n\n\n"<<fichier1.getNom()<<";"<<fichier1.getTaille()<<";"<<fichier1.getMd5()<<std::endl;
    std::cout<<fichier2.getNom()<<";"<<fichier2.getTaille()<<";"<<fichier2.getMd5()<<std::endl;
    if(fichier1.is_same(fichier2))
        std::cout<<fichier1.getNom()<<"=="<<fichier2.getNom()<<std::endl;
    




    return 0;
}