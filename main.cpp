
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <limits>
#include <algorithm>
#include <cctype>

using namespace std;

// Class to store item information
class Item
{
public:
    int id;
    string name, category, description;
    string location, contact, status;
};

// Main system class
class LostFoundSystem
{
private:

    // Read records from a file
    vector<Item> readItems(string filename)
    {
        vector<Item> items;
        ifstream file(filename);
        string line;

        while (getline(file, line))
        {
            stringstream ss(line);
            Item item;
            string id;

            getline(ss, id, '|');
            getline(ss, item.name, '|');
            getline(ss, item.category, '|');
            getline(ss, item.description, '|');
            getline(ss, item.location, '|');
            getline(ss, item.contact, '|');
            getline(ss, item.status, '|');

            if (!id.empty())
            {
                item.id = stoi(id);
                items.push_back(item);
            }
        }

        file.close();
        return items;
    }

    // Save a new record
    void saveItem(string filename, Item item)
    {
        ofstream file(filename, ios::app);

        file << item.id << "|"
             << item.name << "|"
             << item.category << "|"
             << item.description << "|"
             << item.location << "|"
             << item.contact << "|"
             << item.status << endl;

        file.close();
    }

    // Rewrite file after updating a record
    void updateFile(string filename, vector<Item> items)
    {
        ofstream file(filename);

        for (int i = 0; i < items.size(); i++)
        {
            file << items[i].id << "|"
                 << items[i].name << "|"
                 << items[i].category << "|"
                 << items[i].description << "|"
                 << items[i].location << "|"
                 << items[i].contact << "|"
                 << items[i].status << endl;
        }

        file.close();
    }

    // Generate a unique ID
    int getNextId()
    {
        vector<Item> lost = readItems("lost_items.txt");
        vector<Item> found = readItems("found_items.txt");

        int maxId = 0;

        for (int i = 0; i < lost.size(); i++)
            maxId = max(maxId, lost[i].id);

        for (int i = 0; i < found.size(); i++)
            maxId = max(maxId, found[i].id);

        return maxId + 1;
    }

    // Convert text to lowercase for comparison
    string toLowerCase(string str)
    {
        transform(str.begin(), str.end(), str.begin(),
                  [](unsigned char c)
                  {
                      return tolower(c);
                  });

        return str;
    }

    // Display item details
    void displayItems(vector<Item> items)
    {
        if (items.empty())
        {
            cout << "\nNo records available!\n";
            return;
        }

        for (int i = 0; i < items.size(); i++)
        {
            cout << "\n-----------------------------";
            cout << "\nID: " << items[i].id;
            cout << "\nItem: " << items[i].name;
            cout << "\nCategory: " << items[i].category;
            cout << "\nDescription: " << items[i].description;
            cout << "\nLocation: " << items[i].location;
            cout << "\nContact: " << items[i].contact;
            cout << "\nStatus: " << items[i].status;
            cout << "\n-----------------------------\n";
        }
    }

public:

    // Report lost or found item
    void reportItem(bool isLost)
    {
        Item item;

        item.id = getNextId();

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "\nEnter Item Name: ";
        getline(cin, item.name);

        cout << "Enter Category: ";
        getline(cin, item.category);

        cout << "Enter Description: ";
        getline(cin, item.description);

        cout << "Enter Location: ";
        getline(cin, item.location);

        cout << "Enter Contact: ";
        getline(cin, item.contact);

        if (isLost)
        {
            item.status = "Lost";
            saveItem("lost_items.txt", item);
            cout << "\nLost item reported successfully!";
        }
        else
        {
            item.status = "Available";
            saveItem("found_items.txt", item);
            cout << "\nFound item reported successfully!";
        }

        cout << "\nYour Item ID is: " << item.id << endl;
    }

    // View all lost items
    void viewLostItems()
    {
        cout << "\n===== LOST ITEMS =====\n";
        displayItems(readItems("lost_items.txt"));
    }

    // View all found items
    void viewFoundItems()
    {
        cout << "\n===== FOUND ITEMS =====\n";
        displayItems(readItems("found_items.txt"));
    }

    // Search for possible matches
    void searchMatches()
    {
        vector<Item> lost = readItems("lost_items.txt");
        vector<Item> found = readItems("found_items.txt");

        bool matchFound = false;

        cout << "\n===== POSSIBLE MATCHES =====\n";

        for (int i = 0; i < lost.size(); i++)
        {
            for (int j = 0; j < found.size(); j++)
            {
                if (toLowerCase(lost[i].category) ==
                    toLowerCase(found[j].category))
                {
                    cout << "\nPossible Match Found!";
                    cout << "\nLost Item: " << lost[i].name;
                    cout << "\nFound Item: " << found[j].name;
                    cout << "\nFound Item ID: " << found[j].id;
                    cout << "\nFound Location: " << found[j].location;
                    cout << "\nStatus: " << found[j].status << endl;

                    matchFound = true;
                }
            }
        }

        if (!matchFound)
            cout << "\nNo matching items found!\n";

        cout << "\nNote: Matches are based on category only.\n";
    }

    // Request to claim a found item
    void claimItem()
    {
        vector<Item> found = readItems("found_items.txt");

        if (found.empty())
        {
            cout << "\nNo found items available!";
            return;
        }

        displayItems(found);

        int id;
        cout << "\nEnter Item ID to claim: ";
        cin >> id;

        bool foundItem = false;

        for (int i = 0; i < found.size(); i++)
        {
            if (found[i].id == id &&
                found[i].status == "Available")
            {
                found[i].status = "Claim Requested";
                foundItem = true;
                break;
            }
        }

        if (foundItem)
        {
            updateFile("found_items.txt", found);
            cout << "\nClaim request submitted successfully!";
        }
        else
        {
            cout << "\nInvalid ID or item is already claimed!";
        }
    }
};

// Main function
int main()
{
    LostFoundSystem system;
    int choice;

    do
    {
        cout << "\n\n================================";
        cout << "\n CAMPUS LOST & FOUND SYSTEM";
        cout << "\n================================";

        cout << "\n1. Report Lost Item";
        cout << "\n2. Report Found Item";
        cout << "\n3. View Lost Items";
        cout << "\n4. View Found Items";
        cout << "\n5. Search Matching Items";
        cout << "\n6. Claim an Item";
        cout << "\n0. Exit";

        cout << "\n\nEnter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            system.reportItem(true);
            break;

        case 2:
            system.reportItem(false);
            break;

        case 3:
            system.viewLostItems();
            break;

        case 4:
            system.viewFoundItems();
            break;

        case 5:
            system.searchMatches();
            break;

        case 6:
            system.claimItem();
            break;

        case 0:
            cout << "\nThank you for using the system!";
            break;

        default:
            cout << "\nInvalid choice! Try again.";
        }

    } while (choice != 0);

    return 0;
}