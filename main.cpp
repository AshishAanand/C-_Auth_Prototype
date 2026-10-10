#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cstdio> // Required for remove() and rename()

using namespace std;

class Auth
{
public:
    void signUp(string username, string password, string email);
    void signIn(string username, string password);
    void signOut(string username, string password);
};

int main()
{
    // Variable
    Auth auth;
    string username, email, password;

    while (true)
    {
        try
        {
            int option;
            //Steps:
            //1 -> Create the welcome and input screen
            cout << "###########################" << endl;
            cout << "1. Signin" << endl;
            cout << "2. Signup" << endl;
            cout << "3. Signout" << endl;
            cout << "4. Exit" << endl;
            cout << "############################" << endl;

            cout << "What do you want: ";
            cin >> option;


            switch (option)
            {
            case 1:
                //2 -> Implement SignIn feature
                cout << "Enter username: ";
                cin >> username;
                cout << "Enter password: ";
                cin >> password;
                auth.signIn(username, password);
                cout << "User signed" << endl;
                break;
            case 2:
                //3 -> Implement SignUp feature
                cout << "Enter username: ";
                cin >> username;
                cout << "Enter email: ";
                cin >> email;
                cout << "Enter password: ";
                cin >> password;
                auth.signUp(username, password, email);
                cout << "Username saved." << endl;
                break;
            case 3:
                //4 -> Implement SignOut feature
                cout << "Enter username: ";
                cin >> username;
                cout << "Enter password: ";
                cin >> password;
                auth.signOut(username, password);
                cout << "User deleted" << endl;
                break;
            case 4:
                // exit programe
                exit(0);
            }
        }
        catch (const std::runtime_error& error)
        {
            std::cerr << "Caught an exception: " << error.what() << "\n";
        }
    }

    return 0;
}

void Auth::signUp(string username, string password, string email)
{
    // opening the data file
    ofstream openfile("auth_data.txt", ios::app);

    // writing those data into the file
    if (openfile.is_open())
    {
        openfile << username << " " << password << " " << email << endl;
        cout << "User saved";
        openfile.close();
    }
}

void Auth::signIn(string username, string password)
{
    // 1. Open the file strictly for reading (Removed ios::app)
    ifstream openfile("auth_data.txt");

    if (!openfile.is_open())
    {
        cerr << "Error: Could not open auth_data.txt to verify credentials.\n";
        return;
    }

    string fileUsername;
    string filePassword;
    bool loginSuccessful = false;

    // 2. Read the file pairs line-by-line until the end of the file
    while (openfile >> fileUsername >> filePassword)
    {
        // 3. Check if the current line matches the user's input
        if (fileUsername == username && filePassword == password)
        {
            loginSuccessful = true;
            break; // Stop reading the file, we found a match!
        }
    }

    // 4. Close the file stream explicitly
    openfile.close();

    // 5. Output results based on the match status
    if (loginSuccessful)
    {
        cout << "Access Granted! Welcome back, " << username << ".\n";
        // TODO: Update a boolean flag in your Auth class (e.g., isLoggedIn = true;)
    }
    else
    {
        cout << "Access Denied! Incorrect username or password.\n";
    }
}


void Auth::signOut(string username, string password)
{
    // 1. Open the original file to read and a temporary file to write to
    ifstream openfile("auth_data.txt");
    ofstream tempfile("temp_auth.txt");

    if (!openfile.is_open())
    {
        cerr << "Error: Could not open auth_data.txt to process sign-out.\n";
        return;
    }
    if (!tempfile.is_open())
    {
        cerr << "Error: Could not create temporary file.\n";
        openfile.close();
        return;
    }

    string fileUsername;
    string filePassword;
    bool accountRemoved = false;

    // 2. Read all user pairs line by line
    while (openfile >> fileUsername >> filePassword)
    {
        // 3. If BOTH the username and password match, skip writing them (Deletes them)
        if (fileUsername == username && filePassword == password)
        {
            accountRemoved = true;
            continue; // Skip writing this line to the temporary file
        }

        // Keep all other user credentials intact
        tempfile << fileUsername << " " << filePassword << "\n";
    }

    // 4. CRUCIAL: Close both files so Windows releases the file locks
    openfile.close();
    tempfile.close();

    // 5. Apply the changes to the disk storage
    if (accountRemoved)
    {
        remove("auth_data.txt"); // Delete old file
        rename("temp_auth.txt", "auth_data.txt"); // Rename temp to original
        cout << "Successfully signed out and removed credentials for " << username << ".\n";
    }
    else
    {
        remove("temp_auth.txt"); // Clean up the empty temp file if no match was found
        cout << "Sign-out failed: Invalid username or password combination.\n";
    }
}
