#include "Restaurant.h"

void Restaurant :: displayInfo (){

    cout << "Name : " << name <<"\n Adress : " << adress << "\nPhone Number : " << PhoneNumber <<"\n" <<"PrepTime : " << PrepTime <<'\n';
    cout <<Description <<"  ";


    if(active) cout << "\x1b[32m" << "Active" << "\x1b[0m" << endl;
    else cout << "\x1b[31m" << "NotActive" << "\x1b[0m" << endl;
}
bool Restaurant :: getActive ()
{
    return active;
}

int Restaurant :: getID ()
{
    return id;
}