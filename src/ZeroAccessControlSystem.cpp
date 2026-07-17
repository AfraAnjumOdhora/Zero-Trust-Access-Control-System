#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <ctime>

using namespace std;


// USER CLASS
// Stores user information
class User
{
public:
    string name;
    string password;
    string role;

    // Constructor
    User(string n, string p, string r)
    {
        name = n;
        password = p;
        role = r;
    }
};


// LOGGER CLASS
// Handles audit log file
class Logger
{
private:

    // Private encryption function
    // Encapsulation concept
    string encrypt(string text)
    {
        char key = 'K';

        for(int i = 0; i < text.size(); i++)
        {
            text[i] = text[i] ^ key;
        }

        return text;
    }


public:

    // Write encrypted log
    // Abstraction concept
    void write(string message)
    {
        ofstream file("audit.log", ios::app);

        if(file)
        {
            file << encrypt(message) << endl;
            file.close();
        }
    }


    // View decrypted logs
    void viewLogs()
    {
        ifstream file("audit.log");

        cout << "\n===== SECURITY LOGS =====\n";


        if(!file)
        {
            cout<<"No logs found\n";
            return;
        }


        string line;
        char key='K';


        while(getline(file,line))
        {
            for(int i=0;i<line.size();i++)
            {
                line[i]=line[i]^key;
            }

            cout<<line<<endl;
        }


        file.close();
    }

};



// SESSION CLASS
// Controls user session

class Session
{

public:

    bool active;


    Session()
    {
        active=true;
    }



    // Thread + Lambda
    // Auto logout after time
    void start(int time)
    {

        thread t([this,time]()
        {

            this_thread::sleep_for(
                chrono::seconds(time)
            );


            active=false;


        });


        t.detach();

    }

};




// HELPER FUNCTIONS

// Generate OTP
int generateOTP()
{
    return 1000 + rand()%9000;
}



// Search user from vector
// Vector concept

User* findUser(vector<User> &users,string username)
{

    for(int i=0;i<users.size();i++)
    {

        if(users[i].name==username)
        {
            return &users[i];
        }

    }


    return NULL;

}

// ROLE BASED MENU

void showMenu(User &user, Session &session, Logger &log,
              map<string,int> &denyCount)
{

    while(session.active)
    {

        cout<<"\n========== MENU ==========\n";

        cout<<"User : "<<user.name<<endl;
        cout<<"Role : "<<user.role<<endl;


        if(user.role=="Admin")
        {
            cout<<"1. Access File\n";
            cout<<"2. Access Database\n";
            cout<<"3. View Logs\n";
            cout<<"4. Logout\n";
        }


        else if(user.role=="User")
        {
            cout<<"1. Access File\n";
            cout<<"2. Access Database\n";
            cout<<"3. Logout\n";
        }


        else
        {
            cout<<"1. View Public Info\n";
            cout<<"2. Logout\n";
        }



        int choice;

        cout<<"Choice: ";
        cin>>choice;



        // Session verification

        if(!session.active)
        {
            cout<<"\nSession Expired!\n";


            string pass;

            cout<<"Enter Password Again: ";
            cin>>pass;


            if(pass==user.password)
            {
                cout<<"Re-authentication Successful\n";

                session.active=true;
                session.start(20);

                log.write(
                user.name+" re-authentication successful"
                );

            }

            else
            {
                cout<<"Authentication Failed\n";

                log.write(
                user.name+" re-authentication failed"
                );

                return;
            }

        }



        bool access=false;



        // ========== RBAC RULE ==========


        if(user.role=="Admin")
        {

            if(choice==1 || choice==2)
            {
                access=true;
            }


            else if(choice==3)
            {
                log.viewLogs();
                continue;
            }


            else if(choice==4)
            {
                cout<<"Logout successful\n";
                return;
            }

        }



        else if(user.role=="User")
        {

            if(choice==1)
            {
                access=true;
            }


            else if(choice==2)
            {
                access=false;
            }


            else if(choice==3)
            {
                cout<<"Logout successful\n";
                return;
            }

        }



        else
        {

            if(choice==1)
            {
                access=true;
            }


            else if(choice==2)
            {
                cout<<"Logout successful\n";
                return;
            }

        }




        // ========== ACCESS DECISION ==========


        if(access)
        {

            cout<<"Access Granted\n";


            log.write(
            user.name+" Access Granted"
            );

        }



        else
        {

            cout<<"Access Denied\n";


            denyCount[user.name]++;


            log.write(
            user.name+" Access Denied"
            );



            // Suspicious Activity Detection

            if(denyCount[user.name]>=2)
            {

                string risk;


                if(denyCount[user.name]>=4)
                    risk="HIGH";

                else if(denyCount[user.name]>=3)
                    risk="MEDIUM";

                else
                    risk="LOW";



                cout<<"Suspicious Activity Detected\n";

                cout<<"Risk Level : "<<risk<<endl;



                log.write(
                user.name+
                " Suspicious Activity Risk: "
                +risk
                );

            }

        }


    }

}




// ================= LOGIN SYSTEM =================


void login(vector<User> &users,
           Logger &log,
           map<string,int> &failed,
           map<string,bool> &locked,
           map<string,int> &denyCount)
{


    string username;


    cout<<"\nUsername: ";
    cin>>username;



    User *user=findUser(users,username);



    if(user==NULL)
    {
        cout<<"User not found\n";
        return;
    }



    // Account lock checking

    if(locked[username])
    {
        cout<<"Account Locked\n";
        return;
    }




    // Password verification

    bool correct=false;



    for(int i=1;i<=3;i++)
    {

        string password;


        cout<<"Password: ";
        cin>>password;



        if(password==user->password)
        {
            correct=true;
            break;
        }


        else
        {

            cout<<"Wrong Password\n";


            failed[username]++;


            if(failed[username]>=3)
            {

                locked[username]=true;


                cout<<"Account Locked\n";


                log.write(
                username+
                " account locked"
                );


                return;

            }

        }

    }



    if(!correct)
        return;




    // ========== MFA OTP ==========


    int otp=generateOTP();


    cout<<"OTP : "<<otp<<endl;


    int input;


    cout<<"Enter OTP: ";
    cin>>input;



    if(input!=otp)
    {

        cout<<"Wrong OTP\n";


        log.write(
        username+
        " OTP failed"
        );


        return;

    }



    cout<<"Login Successful\n";


    log.write(
    username+
    " login successful"
    );



    // Session creation

    Session session;


    session.start(20);



    showMenu(
    *user,
    session,
    log,
    denyCount
    );


}

// ================= MAIN FUNCTION =================

int main()
{

    srand(time(0));



    // Vector stores users
    // OOP + STL Vector

    vector<User> users;


    users.push_back(
        User("admin","123","Admin")
    );


    users.push_back(
        User("user","123","User")
    );


    users.push_back(
        User("guest","123","Guest")
    );



    // Map stores security information

    map<string,int> failedAttempts;

    map<string,bool> lockedAccounts;

    map<string,int> deniedCount;



    // Logger object

    Logger logger;



    int choice;



    do
    {

        cout<<"\n==============================\n";
        cout<<" ZERO TRUST ACCESS CONTROL SYSTEM\n";
        cout<<"==============================\n";


        cout<<"1. Login\n";
        cout<<"2. Exit\n";


        cout<<"Choice: ";
        cin>>choice;




        if(choice==1)
        {

            login(
                users,
                logger,
                failedAttempts,
                lockedAccounts,
                deniedCount
            );

        }



    }

    while(choice!=2);



    cout<<"\nSystem Closed\n";


    return 0;

}
