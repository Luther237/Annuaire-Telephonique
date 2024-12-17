#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include <unistd.h>

    using namespace std;

    struct Contact{
        string nom;
        string numero;
    };
    vector<Contact> liste;

    void lockfile(){
        string filePath = "fichier.txt";
        string command = "chmod 444 " + filePath;
        system(command.c_str());
    }

    void unlockfile(){
        string filePath = "fichier.txt";
        string command = "chmod u+w " + filePath;
        system(command.c_str());
    }

    void savefile(){
        unlockfile();
        ofstream f("fichier.txt");
        if (f.is_open()){
            f.clear();
            for (int i = 0; i < liste.size(); i++){
                f<<liste[i].nom<<endl;
                f<<liste[i].numero << "\n" <<endl;
            }
        }else{
            cout<<"Erreur lors de l'ouverture du fichier"<<endl;
        }
        f.close();
        lockfile();
    }
    void ajouter(string newnom, string newnumero){
        unlockfile();
        Contact newcontact = {newnom, newnumero};
        liste.push_back(newcontact);
        savefile();
        lockfile();
    }

    void loadfile(){
        unlockfile();
        ifstream f("fichier.txt");
        string line; int i(0); Contact contact;
        if(f.is_open()){
            while (getline(f >> ws, line)){
                    if(i == 0){
                        contact.nom = line;
                        i++;
                    }else if(i == 1){
                        contact.numero = line;
                        liste.push_back(contact);
                        i = 0;
                        continue; 
                    }     
            }
        }else{
            cout<<"Erreur lors de l'ouverture du fichier"<<endl;
        }
        f.close();
        lockfile();
    }

    void lister(){
        if(!liste.empty()){
            for(int i(0); i < liste.size(); i++){
                cout<<liste[i].nom<<" : " <<liste[i].numero<<endl<<endl;
            }
        }else{
            cout<<"Annuaire vide"<<endl;
        }
    }

    int rechercher(string fsearch){
        int nombre_result(0), i;
        for( i = 0; i < liste.size(); i++){
            if (fsearch == liste[i].nom || fsearch == liste[i].numero){
                cout<<"Resultat \n"<<nombre_result << " - " << liste[i].nom << " : " << liste[i].numero<<endl<<endl;
                break;
            }
        }
        return i;
    }

    void modifier(int index, int choix){
        unlockfile();
        string setnom, setnumero; 
        if(choix == 1){
            cout<<"Entrer le nouveau nom: ";
            cin >> setnom;
            liste[index].nom = setnom;
        }else if (choix == 2){
            cout<<"Entrer le nouveau numero: ";
            cin >> setnumero;
            liste[index].nom = setnom;
        }
        savefile();
        cout<<"Contact modifié avec succès"<<endl<<endl;
        lockfile();
    }

    void deletecontact(int index){
        unlockfile();
        liste.erase(liste.begin() + index);
        savefile();
        cout<<"Contact effacé avec succès"<<endl;
        lockfile();
    }
int main(){
    loadfile();
    string newnom, newnumero, search, tresult;
    char delconfirm;
    int choix, modifparametre,indicesearch;
    
    cout<<"BIENVENU DANS VOTRE ANNUAIRE TELEPHONIQUE \n"<<endl;
    do{
        cout<<"SELECTIONNER UNE ACTION \n 1- liste des contacts \n 2- Ajouter un contact \n 3- Supprimer un contact \n 4- Rechercher un contact \n 5- Modifier un contact \n 6- Fermer \n choix: ";
        cin >> choix;
        cout<<"\n";

        switch (choix){
        case 1:
            unlockfile();
            lister();
            lockfile();
            break;
        case 2:
            cout<<"Nom complet: ";
            cin >> newnom;
            cout<<"Numéro: ";
            cin >> newnumero;
            ajouter(newnom, newnumero);
            sleep(1);  
            cout<<"Ajout effectué avec succès!!!"<<endl<<endl;
            break;
        case 3:
            cout<<"Entrer le nom ou le numéro du contact: ";
            cin >> search;
            cout<<"vous voulez supprimer le contact ";
            indicesearch = rechercher(search);
            cout<<"Confirmer (o//n)? ";
            cin >> delconfirm;
            if(delconfirm == 'o'){
                deletecontact(indicesearch);
            }
            break;
        case 4:
            cout<<"1- Entrer le nom ou le numéro du contact: ";
            cin >> search;
            tresult = rechercher(search);
            break;
        case 5:
            cout<<"1- Entrer le nom ou le numéro du contact: ";
            cin >> search;
            cout<<"vous voulez modifier le contact ";
            indicesearch = rechercher(search);
            if (indicesearch == -1){
                cout<<"aucun contact trouvé "<<endl;
            }else{
                cout<<"1- Modifier nom  2- modifier prenom \n choix: ";
                cin >> modifparametre;
                modifier(indicesearch, modifparametre);
            }   
            break;
        case 6:

            break;
        default:
            cout<<"Choix indisponible Recommencez"<<endl;
            break;
        }
    }while (choix != 6);
    
    cout<<"Fin des interactions"<<endl;
        unlockfile();
        savefile();
        lockfile();
        return 0;
}
