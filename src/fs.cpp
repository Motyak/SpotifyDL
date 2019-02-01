#include <iostream>
#include <experimental/filesystem>  //std::experimental::filesystem::directory_iterator -lstdc++fs
#include <cctype>   //toupper
#include <string>   //pop_back

std::vector<std::string> titleToKeywords(std::string& str);
bool rechercher(const std::string& str,const std::vector<std::string>& v);
bool alike(const std::string&,const std::string&);
std::string rmFileExt(std::string);
bool exist( std::string& spo, std::string& path);

//c++17 car filesystem
//g++ -o fs fs.cpp -lstdc++fs
int main()
{
    std::string path="/mnt/e/Tommy\ Anfosso/Desktop/radioBonheur";
    //for(const auto& e : std::experimental::filesystem::directory_iterator(path))
    for(const auto& e : std::experimental::filesystem::recursive_directory_iterator(path))  //affichage récursif
    {
        if(!is_directory(e))    //n'afficher que les noms des morceaux, pas le nom des répertoires.
            std::cout<<rmFileExt(e.path().filename().string())<<std::endl;
    }
    std::string str1="Loverboy - Working For The Weekend";  //loverboy   working for the weekend=>loverboy,working,for,the,weekend
    std::string str2="Working for the weekend-loverboy";    //working for the weekend loverboy=>working,for,the;weekend,loverboy


    return 0;
}

std::vector<std::string> titleToKeywords(std::string& str)    //convertit une chaine en mots clés
{
    unsigned i=0;
    for(i;i<str.size();++i)
    {
        std::toupper(str[i]);
        if(str[i]=='('||str[i]==')'||str[i]=='-')
            str[i]==' ';
    }
    std::vector<std::string> mots;
    std::string tmp="";
    for(i=0;i<str.size();++i)
    {
        if(str[i]!=' ') //cas ou il s'agit de lettres ou de caractères autres que '('   ,   ')'     ,   '-'    
            tmp+=str[i];
        else
        {
            if(str[i-1]!=' ')   //(cas ou il y a plusieurs espaces consécutifs=>ne pas ajouter de mots clés vide)
            {
                mots.push_back(tmp);
                tmp="";
            }
        }
    }
    return mots;
}

bool rechercher(const std::string& str,const std::vector<std::string>& v)
{
    for(const auto& e : v)
    {
        if(e==str)
            return true;
    }
    return false;
}

bool alike(std::string& spo, std::string& fic) //majorité des mots clés correspondant
{
    std::vector<std::string> m1=titleToKeywords(spo);
    std::vector<std::string> m2=titleToKeywords(fic);
    unsigned c=0;
    //nb de mots clés présents dans les 2 vector, par rapport a la taille du vector le plus grand. matching ratio, entre 0 et 1
    for(const auto& e : m1)
    {
        if(rechercher(e,m2))
            c++;
    }
    if(c/m1.size()>0.50)    //matching ratio supérieur à 50%
        return true;
    return false;
}

bool exist(const std::string& spo,const std::string& path)  //vérifie si la Musique Spotify existe dans le répertoire
{

}

std::string rmFileExt(std::string str)
{
    unsigned cur=str.size()-1;
    while(str[cur]!='.')
    {
        if(str[cur]!='\0')
        {
            str.pop_back();
            --cur;
        }
    }
    str.pop_back();//
    return str;
}