#include "lib.h"
#include "Menudb.cpp"
#include "Menu.cpp"
#include "Restaurant.cpp"
#include "Restaurantdb.cpp"
#include "Order.h"
#include "Orderdb.cpp"
#include "sqlite3.h"
#include "Login_and_SignUp.cpp"

//g++ main.cpp sqlite3.o -o app.exe -Wall


int Get_Integer()
{
    int input;
    while(!(cin >> input)){
        cout << "The Input is wrong\n";
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    return input;
}

double Get_Double()
{
    double input;
    while(!(cin >> input)){
        cout << "The Input is wrong\n";
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    return input;
}

// -*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*v

void prints(const vector<unique_ptr<MenuItem>> &menitems)
{
    int flag = 1;
    cout <<" ------------- MenuItems --------------\n\n";
    for(const auto &X : menitems){
        cout << "( "<< flag++ << " )"<<" - ";
        X->displayInfo();
        
        cout<<"\n------------------------------------------------------------------------------\n\n";
    }
    
    
}


bool Customer(RestaurantDAO &resdb ,MenuItemDAO &mendb , OrderDAO &Ord , int UserID , OrderItemsDAO &ordItm)
{   

    system("cls");
    cout << " ------------------------ Login successfully ------------------------\n\n";

    cout << " 1- Show Restaurants / Register Order\n 2 - Order History\n";
    
    
    int Num;
    while(true)
    {
        Num = Get_Integer();
        if(Num == 1 || Num == 2)break;
        else cout << "The input is wrong\n";
    }


    while(1){
        if(Num == 1){
            while(true){
                system("cls");

                vector<Restaurant> rests = resdb.getRestaurants(); 

                if(rests.size() == 0){
                    cout << "There are no restaurants available!!!\n";
                    return 0;
                }

                int l = 1;
                for(auto X : rests){
                    cout << "( "<< l++ << " )"<<" - ";
                    X.displayInfo();
                    cout<<"------------------------------------------------------------------------------\n";
                }
                cout <<"\n";
                cout <<"-- Select a Restaurant to place an Order. --\n";
                cout <<"------ Enter 0 to go back one step ------ \n";

                int Number;
                while(true)
                {
                    Number = Get_Integer();
                    if(Number > (int)rests.size() || Number < 0)cout << "The input is wrong\n";
                    else if(Number != 0 && !rests[Number-1].getActive()) cout << "Restaurant Is Inactive\n";
                    else break;
                }
                
                if(Number == 0)return 0;

                
                int id = rests[Number-1].getID();
                
                auto menitems = mendb.MenuForRestaurant(id);

                double price = 0;
                
                
                vector<int> OrderItems; 
        
                while(true){
                    system("cls");
                    prints(menitems);

                    if(menitems.size() == 0){
                        cout <<"\n";
                        cout << "\x1b[31m" <<  "           There are no items.\n\n" << "\x1b[0m" << endl;

                    }

                    cout << "Total Cost : " << price << " Toman \n\n";

                    
                    cout <<"------ Enter 0 to End ------ \n";
                    cout <<"------ Enter -1 to View Shopping Cart ------ \n\n";
                    cout << "Please Enter The Item Number : ";
                    int nm;    
                    
                    while(true)
                    {
                        nm = Get_Integer();
                        if(nm < 0){
                            nm = -1;
                            break;
                        }
                        else if(static_cast<size_t>(nm) > menitems.size() || nm < -1)cout << "The input is wrong\n";
                        else if(nm != 0 && nm != -1 &&!menitems[nm-1]->getAvailable()) cout << "Item Is InAvailable\n";
                        else break;
                    }

                    if(nm == 0 && price != 0){
                        int ORDERID = Ord.AddOrder(UserID , rests[Number-1].getID() , rests[Number-1].getName() , price , "Awaiting restaurant approval"); 

                        int h[400] = {0};

                        for(int C : OrderItems)h[C]++;

                        for(int C : OrderItems)
                            for(const auto &X : menitems)
                                if(X->getID() == C && h[C] != 0){
                                    double TotPric = h[C] * X->getPrice();
                                    ordItm.AddOrderItem(ORDERID , X->getName() , h[C] , TotPric);        
                                    h[C] = 0;
                                }
                                
                            

                        break;
                    }   
                    else if(nm == 0 && price == 0){
                        return 0;
                    }
                    else if(nm == -1){
                        while(1){
                            int l = 1;
                            system("cls");
                            vector<double> prices;

                            for(int C : OrderItems){
                                for(const auto &X : menitems){
                                    if(X->getID() == C){
                                        cout << "( " << l++ << " )";
                                        prices.push_back(X->getPrice());
                                        X->displayInfo();
                                    }
                                }
                            }

                            cout <<"Cost : " << price<<" Toman\n";
                            
                            cout << "Please Enter the item Number you Want to remove:\n";
                            cout <<"------ Enter 0 to go back one step ------ \n";


                            int numberx;
                            while(true)
                            {
                                numberx = Get_Integer();
                                if(numberx > (int)OrderItems.size() || numberx < 0)cout << "The input is wrong\n";
                                else break;
                            } 
                            if(numberx == 0)break;
                            else{
                                
                                price -= prices[numberx-1];
                                OrderItems.erase(OrderItems.begin() + numberx - 1);
                                prices.erase(prices.begin() + numberx - 1);
                            }
                        }
                    }

                    else{
                        price += menitems[nm-1]->getPrice();
                        OrderItems.push_back(menitems[nm-1]->getID());
                    }

                }
            }
        }

        else if(Num == 2){
            system("cls");
            vector<Order> ord = Ord.AllOrders("UserID" , UserID);
            // 0 bashe

            for(auto X : ord){
                X.displayInfo();
                vector<OrderItem> OrderItems = ordItm.GetItemsForOrder(X.getID());
                cout << "\n ---------------------- Order Items ---------------------\n\n";
                for (int Y = 0 ; Y < (int)OrderItems.size() ; Y++)
                {
                    OrderItems[Y].displayInfo();
                }

                cout << "\n\n -------------------------------------------------------------------------------------------------------------\n\n\n";
            }
            

            cout <<"\n";
            cout <<"------ Enter 1 for exit ------ \n";
            cout <<"------ Enter 2 to go back one step ------ \n";

            int numb;
            while(true)
            {
                numb = Get_Integer();
                if(numb != 1 && numb != 2)cout << "Wrong Input!!\n";
                else break;
            } 
            if(numb == 1)return 1;
            else return 0;
        }
        

    }
    return 0;
}

// -*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*v
bool isNumber(const string& s) {
    if (s.empty()) return false;
    for (char c : s) {
        if (!isdigit(c)) return false;
    }
    return true;
}

bool RestaurantOwner(RestaurantDAO &resdb ,MenuItemDAO &mendb , OrderDAO &Ord , int UserID , OrderItemsDAO &ordItm)
{   
    system("cls");
    cout <<"---------------- Your Restaurants ----------------\n\n";
    vector<Restaurant> rests = resdb.getRestaurants();
    vector<Restaurant> UsersREST;
    int l = 1;
    for(auto X : rests){
        if(X.getManagerID() == UserID){
            UsersREST.push_back(X);
            cout <<"( " << l++ << " ) ";
            X.displayInfo();
            cout <<"\n --------------------------------------------------------------------\n";
        }
    }
    if(UsersREST.size() == 0){
        cout << "There is no restaurant available.\n";
        cout << "Enter 0 to exit\n";
        string P;
        cin >> P;
        return 1;
    }
    cout << "Choose a Restaurant : "<<"\n";

    int nm;    
    
    while(true)
    {
        nm = Get_Integer();
        if(nm > (int)rests.size() || nm < 1)cout << "The input is wrong\n";
        else if(UsersREST[nm-1].getActive() == 0)cout << "Restaurant Is Not Active!!!!\n";
        else break;
    }
    system("cls");

    cout << "1 - Change Restaurant Information \n";
    cout << "2 - Add/Change Restaurant Menu Items \n";
    cout << "3 - view/Change Orders \n";

    
    int Numb;
    while(true)
    {
        Numb = Get_Integer();
        if(Numb != 1 && Numb != 2 && Numb != 3)cout << "The input is wrong\n";
        else break;
    }

    Restaurant SelectedRestaurant = UsersREST[nm-1];
    
    if(Numb == 1){
        system("cls");
        cout << "Which one do you want to change?\n";
        cout << "1 - Name    2 - address    3 - PrepTime    4 - PhoneNumber    5 - Description\n";
        int Input;
        while(true)
        {
            Input = Get_Integer();
            if(Input < 1 || Input > 5)cout << "The input is wrong\n";
            else break;
        }

        string NewUpdate = "";
        cout << "Please enter your change : \n";
        
        if(Input == 3){
            cin.ignore();
            getline(cin , NewUpdate);
            while(!isNumber(NewUpdate)){
                cout << "Invalid Input ! \n";
                cin  >> NewUpdate;  
            }
        }

        else{
            cin.ignore();
            getline(cin , NewUpdate);
        }

        resdb.UpdateINFO(SelectedRestaurant.getID() , Input-1 , NewUpdate);
    }

    else if(Numb == 2){
        
        auto menitems = mendb.MenuForRestaurant(SelectedRestaurant.getID());
        prints(menitems);
        if(menitems.size() == 0){
            cout <<"\n";
            cout << "\x1b[31m" <<  "           There are no items.\n\n" << "\x1b[0m" << endl;

        }
        int NumberMenuItem;    
        
        cout << "---- Enter 0 Go Back ----\n";            
        cout << "---- Enter -1 Add New Item ----\n";            
        
        while(true)
        {
            NumberMenuItem = Get_Integer();
            if(NumberMenuItem < 0){
                NumberMenuItem = -1;
                break;
            }
            else if(static_cast<size_t>(NumberMenuItem) > menitems.size() || NumberMenuItem < -1)cout << "The input is wrong\n";
            else break;
        }
        system("cls");
        
        if(NumberMenuItem == 0)return 0;
        if(NumberMenuItem == -1){
            cout << "------------------ New Menu Item -----------------\n\n";
            cin.ignore();
            cout << "Enter Name : "; string Name; getline(cin , Name);
            cout << "Enter description : "; string description; getline(cin , description);

            cout << "Enter price : "; double price; price = Get_Double(); 

            

            cout << "Enter 1/Available . 0/Not Available: "; int available; available = Get_Integer();
            if(available >= 1) available = 1; else available = 0;


            cout << "Enter Type 1/Food . 0/Drink: "; int type; type = Get_Integer();
            if(type >= 1) type = 1; else type = 0;
            
            if(type == 1){
                cout << "Enter CookTime : "; int CookTime; CookTime = Get_Integer();
                mendb.AddMenuItem(SelectedRestaurant.getID() , Name , description , price , available , "Food" , CookTime);
            }

            else{
                cout << "Enter Voulme : "; int Voulme; Voulme = Get_Integer();
                mendb.AddMenuItem(SelectedRestaurant.getID() , Name , description , price , available , "Drink" , Voulme);
            }
            return 0;
        }
        
        cout << "1 - delete Item \n2 - change Item INFO\n";

        int Input = Get_Integer();
        if(Input < 1)Input = 1;
        if(Input > 2)Input = 2;
        

        if(Input == 1){
            
            mendb.DeleateMenuItem(menitems[NumberMenuItem-1]->getID());
        }

        else if(Input == 2){ // Need To FIX
            cin.ignore();
            cout << "Enter New Name : "; string NewName; getline(cin , NewName);

            cout << "Enter New description : "; string Newdescription; getline(cin , Newdescription);

            cout << "Enter New price : "; double NewPrice; NewPrice = Get_Double(); 

            cout << "Enter New 1/Available . 0/Not Available : "; int available; available = Get_Integer();
            if(available >= 1) available = 1; else available = 0;
            cout << "Enter New CookTime for foods / Volume for drinks : "; int CookOrVol; CookOrVol = Get_Integer(); 

            mendb.UpdateItem(menitems[NumberMenuItem-1]->getID() , NewName , Newdescription , NewPrice , available , CookOrVol);
            
        }

        

    }

    else if(Numb == 3){

        vector<Order> ords = Ord.AllOrders("restaurantId" , SelectedRestaurant.getID());
        int l = 1;
        
        for(auto X : ords){
            cout <<" ( " << l++  << " ) ";
            X.displayInfo();
            vector<OrderItem> OrderItems = ordItm.GetItemsForOrder(X.getID());
            cout << "\n ---------------------- Order Items ---------------------\n\n";
            for (int Y = 0 ; Y < (int)OrderItems.size() ; Y++)
            {
                OrderItems[Y].displayInfo();
            }

            cout << "\n\n -------------------------------------------------------------------------------------------------------------\n\n\n";
        }
        if(ords.size() == 0){
            cout << "\x1b[31m" <<  "           There are no orders available.\n\n" << "\x1b[0m" << endl;
        }
        else cout << "Which order status would you like to change???\n";

        
        int OrderNumber;    
        cout << "---- Enter 0 Go Back ----\n";            
        while(true)
        {
            OrderNumber = Get_Integer();
            
            if(OrderNumber > (int)ords.size() || OrderNumber < -1)cout << "The input is wrong\n";
            else break;
        }
        system("cls");
        if(OrderNumber == 0)return 0;
        cout << "Change to : ...\n";
        cout << "1 - Processing\n";
        cout << "2 - Delivered\n";
        int INPUT;  
        INPUT = Get_Integer();
        if(INPUT < 1)INPUT = 1;
        if(INPUT > 2)INPUT = 2;

        if(INPUT == 1)Ord.UpdateStatusOrder("Processing",ords[OrderNumber-1].getID());
        else Ord.UpdateStatusOrder("Delivered",ords[OrderNumber-1].getID());

        
    }

    return 0;
}

// -*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*v


bool Admin(RestaurantDAO &resdb , OrderDAO &Ord , LOGINDAO &dbaslog)
{   
    system("cls");
    cout << "1 - Add New Restaurant\n";
    cout << "2 - Activate/deactivate Restaurants\n";
    cout << "3 - Program Overview\n";

    int INPUT = Get_Integer();
    if(INPUT < 1)INPUT = 1;
    if(INPUT > 3)INPUT = 3;

    if(INPUT == 1){
        system("cls");

        cout << "Enter ManagerID : "; ; int managerID; managerID = Get_Integer();   

        cin.ignore();
        cout << "Enter Name : "; string Name; getline(cin , Name);    
        cout << "Enter adress : "; string adress; getline(cin , adress);
        cout << "Enter 1/active . 0/Not active: "; int active; active = Get_Integer();
        if(active >= 1) active = 1; else active = 0;

        cout << "Enter AveragePrepTime: "; int AveragePrepTime; AveragePrepTime = Get_Integer();
        cin.ignore();
        cout << "Enter PhoneNumber : "; string PhoneNumber; getline(cin , PhoneNumber);    
        cout << "Enter Description : "; string Description; getline(cin , Description);    

        resdb.AddRestaurant( Name , managerID ,adress , active , AveragePrepTime , PhoneNumber , Description);
        
    }
    
    else if(INPUT == 2) {
        vector<Restaurant> AllRestaurant =  resdb.getRestaurants();
        int l = 1;
        for(auto X : AllRestaurant){
            cout << " ( " << l++ <<" ) ";
            X.displayInfo();
        }
        if((int)AllRestaurant.size() == 0){
            cout << "There is no Restaurant available\n";
            return 0;
        }
        cout << "Enter the desired number to change the status : ";
        int RestaurantNumber;
        RestaurantNumber = Get_Integer();

        if(RestaurantNumber > (int)AllRestaurant.size())RestaurantNumber = AllRestaurant.size();
        if(RestaurantNumber < 0) RestaurantNumber = 0;

        Restaurant rest = AllRestaurant[RestaurantNumber-1];

        cout << rest.getActive() <<"\n\n\n";
        resdb.UpdateINFO(rest.getID() , 5 , to_string(1-rest.getActive()));
        
    }

    else{   
        cout << "Total Users : " << dbaslog.getTotalUsers()<<"\n\n";
        cout << "Total Orders : " << Ord.getTotalOrders()<<"\n\n";

        double avarage = (dbaslog.getTotalUsers() * 1.0) / (Ord.getTotalOrders() * 1.0);
        cout << "Average order per user : " << avarage <<"\n\n";

        cout << "Total Cost : " << Ord.getTotalPrice() << "\n\n";
        

        string n;
        cout << "Enter something to exit : ";
        cin >> n;
        
    }
    return 0;
}



void HandleUserType(RestaurantDAO &resdb ,MenuItemDAO &mendb , OrderDAO &Ordb , LOGINDAO &dbaslog , OrderItemsDAO &ordItm)
{

    

    cout << "\n---------------------------------- Welcome ----------------------------------------------\n";
    cout << "Select your role: \n 1 - Customer\n 2 - RestaurantOwner\n 3 - Admin\n";
    int Number;
    while(true){
        Number = Get_Integer();
        if(Number == 1 || Number == 2 || Number == 3)break;
        else cout << "The input is wrong\n";
   }
    
    int UserID; 
    system("cls");

    cout << "1 - sign UP\n";
    cout << "2 - Login\n";

    int Input;
    while(true){
        Input = Get_Integer();
        if(Input >= 2)Input = 2;
        else Input = 1;

        if(Input == 1 && Number == 3)cout << "You cannot register with the admin role.!!!!!";
        else break;
    }

    string ROLES[3] = {"Customer"  , "RestaurantOwner" , "Admin"};

    string Username; 
    string Password;
    
    while(1){
       
        cout <<"Enter Username : ";
        cin >> Username;

       
        cout <<"Enter Password : ";
        cin >> Password;

        const vector<unsigned char> username_bytes = { 41, 46, 44, 40, 43, 38, 51, 42, 52, 43, 38 };
        const vector<unsigned char> password_bytes = { 39, 38, 42, 46, 43, 110, 108, 103, 114 };

        if(Number == 3){

            if(Username != get_hidden_string(username_bytes) || Password != get_hidden_string(password_bytes)){
                cout <<"Wrong UserName/Password!\n"; 
                continue;
            }    
            else{
                cout << "Welcome Admin!!!!!!!\n";
                break;
            }
            
        }

        else if(Input == 1 && Number != 3) {
            if(dbaslog.RegisterUser(Username , Password , ROLES[Number-1]))break;
        }

        else if(Input == 2){
            if(dbaslog.LoginUser(Username , Password))break;
        }
    }
    
    

    UserID = dbaslog.getUserIdByUsername(Username);

    if(Number == 1){

        while(!Customer(resdb , mendb , Ordb , UserID , ordItm));
    }

    else if(Number == 2)
    {
        while(!RestaurantOwner(resdb , mendb , Ordb , UserID , ordItm));
    }
    else{
        while(!Admin(resdb  , Ordb , dbaslog));
    }
}