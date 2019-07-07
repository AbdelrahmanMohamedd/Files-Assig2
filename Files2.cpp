#include <bits/stdc++.h>
using namespace std;

struct Book{
    char isbn[6];
    char title[20];
    char author[20];
    char pages[10];
    char year[5];
    char price[10];
};

const short totalRecSize = 77; // 6 for delimiters
const char delimiter  = '&';
string filename = "Books.txt";
string secondfile = "NewBooks.txt";

void addBook(short &header);
short Search(char isbn[]);
void Delete(short &header);
void Print(char ISBN[]);
void PrintAll();
void update(char ISBN[]);
//Compressfile with ios trunc? no

void addBook(short &header){
    Book temp;
    fstream infile;
    infile.open(filename,ios::in|ios::out|ios::ate); //cursor to the end
    cout << "Enter ISBN:" << endl;
    cin >> temp.isbn;
    cin.ignore();
    cout << "Enter Title: " << endl;
    cin.getline(temp.title, 20);
    cout << "Enter Author's name: " << endl;
    cin.getline(temp.author, 20);
    cout << "Enter number of Pages: " << endl;
    cin >> temp.pages;
    cin.ignore();
    cout << "Enter year of publication: " << endl;
    cin >> temp.year;
    cin.ignore();
    cout << "Enter book's price: " << endl;
    cin >> temp.price;
    cin.ignore();
    infile.seekp(2, ios::end); ///Must start writing after header

    if(header == -1){
        infile.seekp(0, ios::end);
        int length1 = strlen(temp.isbn);
        infile.write(temp.isbn, length1);
        infile.write(&delimiter, 1);


        int length2 = strlen(temp.title);
        infile.write(temp.title, length2);
        infile.write(&delimiter, 1);

        int length3 = strlen(temp.author);
        infile.write(temp.author, length3);
        infile.write(&delimiter, 1);

        int length4 = strlen(temp.pages);
        infile.write(temp.pages, length4);
        infile.write(&delimiter, 1);

        int length5 = strlen(temp.year);
        infile.write(temp.year, length5);
        infile.write(&delimiter, 1);

        int length6 = strlen(temp.price);
        infile.write(temp.price, length6);
        infile.write(&delimiter, 1);

        int currRecSize = length1 + length2 + length3 + length4 + length5 + length6 + 6; /// 6 bytes of 6 delimiters.
        char recordCompleter = '-';

        if ( currRecSize < totalRecSize){
            for(int z = 0; z < (totalRecSize - currRecSize); z++){
                infile.write((char*)&recordCompleter, 1);   ///bkmlo b ---
            }
        }


    }else{
        short tempHeader = 0;
        infile.seekg(header*totalRecSize + 3, ios::beg);
        ///seek from beginning, cursor meets 2 bytes of header and 1 byte of this deleted record, so +3 to skip them
        infile.read((char*)&tempHeader, sizeof(tempHeader));
        /// store value of del record's header in the initial header to seek the record deleted before it
        infile.seekp(header*totalRecSize + 2, ios::beg);
        /** move write cursor to position of deleted record Char and header, to replace them with new data*/

        int length1 = strlen(temp.isbn);
        infile.write(temp.isbn, length1);
        infile.write(&delimiter, 1);

        int length2 = strlen(temp.title);
        infile.write(temp.title, length2);
        infile.write(&delimiter, 1);

        int length3 = strlen(temp.author);
        infile.write(temp.author, length3);
        infile.write(&delimiter, 1);

        int length4 = strlen(temp.pages);
        infile.write(temp.pages, length4);
        infile.write(&delimiter, 1);

        int length5 = strlen(temp.year);
        infile.write(temp.year, length5);
        infile.write(&delimiter, 1);

        int length6 = strlen(temp.price);
        infile.write(temp.price, length6);
        infile.write(&delimiter, 1);

        int currRecSize = length1 + length2 + length3 + length4 + length5 + length6 + 6; /// 6 bytes of 6 delimiters.
        char recordCompleter = '-';

        if ( currRecSize < totalRecSize){
            for(int z = 0; z < (totalRecSize - currRecSize); z++){
                infile.write((char*)&recordCompleter, 1);
            }
        }

        infile.seekp(0,ios::beg);
        infile.write((char*)&tempHeader, sizeof(tempHeader));///Header points to record deleted before the deleted record I just replaced
        /// Assuming -1 header points to the end of the file.
        header = tempHeader; ///Now header points to the previous deleted (free) record
    }
    infile.close();

}

short Search(char isbn[]){

    char tempISBN[6];
    fstream file;
    file.open(filename, ios::in|ios::out);
    file.seekg(-2, ios::end);
    short fileSize = (short)file.tellg();
    ///-2 for the extra header at start; other headers aren't extra bec replace existing bytes
    fileSize = (short) fileSize;
    for(short i = 0; i < fileSize/totalRecSize; i++){ ///Get number of records and iterate to find desired record
        file.seekg(i*totalRecSize +2, ios::beg);
        file.getline(tempISBN, 6, '&');
        file.ignore(1, ios::cur);

        if(tempISBN[0] == '*') {
            continue;}

        cout << "required isbn:" << isbn<<endl;
        cout << "current isbn:" << tempISBN<<endl;

        if (isbn[0]!=tempISBN[0] && isbn[1]!=tempISBN[1] && isbn[2]!=tempISBN[2] && isbn[3]!=tempISBN[3] && isbn[4]!=tempISBN[4] )
        {
            cout << "not yet\n\n";
            continue;
        }
        cout << "Found at index" << i << endl;
        return i; ///If isbn inputted = isbn found (thus SEARCHED) then return the record's number
    }
    cout << "Not found" << endl;
    return -1; ///Kept doing "continue" and never found
}

void Delete(short &header){
    fstream file;
    file.open(filename, ios::in|ios::out);

    char tempISBN[6];
    char deleter='*';
    cout << "Enter book's ISBN to delete it:";
    cin >> tempISBN;
    cin.ignore();

    short toDelete = (short)Search(tempISBN);
    if(toDelete == -1){
        cout << "\nRecord doesn't exist or is already deleted" << endl;
    }else{
        file.seekp(toDelete*totalRecSize +2 ,ios::beg);
        file.write(&deleter, 1); /// ///////////write the deletion char at the start of the record
        file.write((char*)&header, sizeof(header));
        /// Header used as func parameter is header of a PREVIOUSlY deleted record; changes because passed by ref

        header = (short)toDelete; /// Number of currently deleted record
        file.seekp(0, ios::beg);
        file.write((char*)&header, sizeof(header));
    }
    file.close();
}

void Print(char ISBN[]){
    string ID(ISBN);
    string Title, tempstring, Author, pages, year, price;
    fstream file;
    file.open(filename, ios::in|ios::out);
    short toPrint = Search(ISBN);
    if(toPrint == -1){
        cout << "\nDeleted record or non-existent record; cannot be printed\n";
    }else{
        file.seekg(toPrint*totalRecSize+2,ios::beg);
        getline(file, ID, '&');
        getline(file, Title, '&');
        getline(file, Author, '&');
        getline(file, pages, '&');
        getline(file, year, '&');
        getline(file, price, '&');
        cout << "ISBN: " << ISBN << endl;
        cout << "Title: " << Title << endl;
        cout << "Author: " << Author << endl;
        cout << "Pages: " << pages << endl;
        cout << "Year: " << year << endl;
        cout << "Price: " << price << endl;
    }
    file.close();
}

void PrintAll(){
    string ID, Title, Author, pages, year, price;
    fstream file;
    file.open(filename, ios::in|ios::out);

    char delCheck[1]; /// May change declaration
    file.seekg(0, ios::end);
    int fileSize = file.tellg(); ///may add -2

    for(int i = 0; i < fileSize/totalRecSize; i++){
        file.seekg(i*totalRecSize+2,ios::beg);
        file.read((char*)&delCheck, 1);

        if(delCheck[0] == '*'){
                continue;
        }
        file.seekg(0, ios::beg);
        cout << "\nCursor position before read: " << i*totalRecSize+2 << endl;
        file.seekg(i*totalRecSize+2,ios::beg);
        getline(file, ID, '&');
        getline(file, Title, '&');
        getline(file, Author, '&');
        getline(file, pages, '&');
        getline(file, year, '&');
        getline(file, price, '&');
        cout << "ISBN: " << ID << endl;
        cout << "Title: " << Title << endl;
        cout << "Author: " << Author << endl;
        cout << "Pages: " << pages << endl;
        cout << "Year: " << year << endl;
        cout << "Price: " << price << endl;
        ID="";
        Title="";
        Author.clear();
        pages.clear();
        year.clear();
        price.clear();
        cout << "\nCursor position after read: " << file.tellg() << endl;
        cout << "\nIteration no.: " << i << endl;
        cout<< "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
    }file.close();
}

void update(char ISBN[]){
    Book temp;
    fstream infile;
    infile.open(filename, ios::in|ios::out);

    int toUpdate = Search(ISBN);
    if(toUpdate == -1){
        cout <<"Deleted/non-existent record; cannot update\n";
    }else{
    infile.seekp(toUpdate*totalRecSize+2, ios::beg);

    cout << "Enter ISBN:" << endl;
    cin >> temp.isbn;
    cin.ignore();
    cout << "Enter Title: " << endl;
    cin.getline(temp.title, 20);
    cout << "Enter Author's name: " << endl;
    cin.getline(temp.author, 20);
    cout << "Enter number of Pages: " << endl;
    cin >> temp.pages;
    cin.ignore();
    cout << "Enter year of publication: " << endl;
    cin >> temp.year;
    cin.ignore();
    cout << "Enter book's price: " << endl;
    cin >> temp.price;
    cin.ignore();

        int length1 = strlen(temp.isbn);
        infile.write(temp.isbn, length1);
        infile.write(&delimiter, 1);


        int length2 = strlen(temp.title);
        infile.write(temp.title, length2);
        infile.write(&delimiter, 1);

        int length3 = strlen(temp.author);
        infile.write(temp.author, length3);
        infile.write(&delimiter, 1);

        int length4 = strlen(temp.pages);
        infile.write(temp.pages, length4);
        infile.write(&delimiter, 1);

        int length5 = strlen(temp.year);
        infile.write(temp.year, length5);
        infile.write(&delimiter, 1);

        int length6 = strlen(temp.price);
        infile.write(temp.price, length6);
        infile.write(&delimiter, 1);

        int currRecSize = length1 + length2 + length3 + length4 + length5 + length6 + 6; /// 6 bytes of 6 delimiters.
        char recordCompleter = '-';

        if ( currRecSize < totalRecSize){
            for(int z = 0; z < (totalRecSize - currRecSize); z++){
                infile.write((char*)&recordCompleter, 1);
            }
        }
    }
    infile.close();
}

void CompressFile(short &header){
    Book b;
    string ID, Title, Author, pages, year, price;
    header = -1;
    fstream oldfile, newfile;
    oldfile.open(filename, ios::in|ios::out);
    newfile.open(secondfile, ios::in| ios::out);

    char delCheck[1];
    oldfile.seekg(0, ios::end);
    int fileSize = oldfile.tellg();

    oldfile.seekg(0, ios::beg);
    newfile.write((char*)&header, sizeof(header)); /// Write header in newfile
    for(int i = 0; i < fileSize/totalRecSize; i++){ /// Loop that fills temporary file
        oldfile.seekg(i*totalRecSize+2,ios::beg);
        oldfile.read((char*)&delCheck, 1);
        if(delCheck[0] == '*'){
            continue;
        }else{
            oldfile.seekg(i*totalRecSize+2,ios::beg);
            getline(oldfile, ID, '&');
            getline(oldfile, Title, '&');
            getline(oldfile, Author, '&');
            getline(oldfile, pages, '&');
            getline(oldfile, year, '&');
            getline(oldfile, price, '&');

            newfile.seekp(i*totalRecSize+2,ios::beg); ///seek writing after header
            int length1 = strlen(ID.c_str());
            newfile.write(ID.c_str(), length1);
            newfile.write(&delimiter, 1);
            int length2 = strlen(Title.c_str());
            newfile.write(Title.c_str(), length2);
            newfile.write(&delimiter, 1);
            int length3 = strlen(Author.c_str());
            newfile.write(Author.c_str(), length3);
            newfile.write(&delimiter, 1);
            int length4 = strlen(pages.c_str());
            newfile.write(pages.c_str(), length4);
            newfile.write(&delimiter, 1);
            int length5 = strlen(year.c_str());
            newfile.write(year.c_str(), length5);
            newfile.write(&delimiter, 1);
            int length6 = strlen(price.c_str());
            newfile.write(price.c_str(), length6);
            newfile.write(&delimiter, 1);

            int currRecSize = length1 + length2 + length3 + length4 + length5 + length6 + 6; /// 6 bytes of 6 delimiters.
            char recordCompleter = '-';
            if ( currRecSize < totalRecSize){
                for(int z = 0; z < (totalRecSize - currRecSize); z++){
                    newfile.write((char*)&recordCompleter, 1);
                }
            }
        ID="";
        Title="";
        Author.clear();
        pages.clear();
        year.clear();
        price.clear();
        }
    }
    oldfile.close();
    oldfile.open(filename, ios::out|ios::trunc);
    oldfile.close();
    oldfile.open(filename, ios::out|ios::out);
    ///Truncate oldfile, read newfile, dump in oldfile
    oldfile.write((char*)&header, sizeof(header));

    newfile.seekg(0, ios::end);
    fileSize = newfile.tellg(); ///Update number of records

        for(int j = 0; j < fileSize/totalRecSize; j++){
            newfile.seekg(j*totalRecSize+2,ios::beg);
            getline(newfile, ID, '&');
            getline(newfile, Title, '&');
            getline(newfile, Author, '&');
            getline(newfile, pages, '&');
            getline(newfile, year, '&');
            getline(newfile, price, '&');
            int length1 = strlen(ID.c_str());     ///c_str: returns A pointer to the c-string representation of the string object's value.
            oldfile.write(ID.c_str(), length1);
            oldfile.write(&delimiter, 1);
            int length2 = strlen(Title.c_str());
            oldfile.write(Title.c_str(), length2);
            oldfile.write(&delimiter, 1);
            int length3 = strlen(Author.c_str());
            oldfile.write(Author.c_str(), length3);
            oldfile.write(&delimiter, 1);
            int length4 = strlen(pages.c_str());
            oldfile.write(pages.c_str(), length4);
            oldfile.write(&delimiter, 1);
            int length5 = strlen(year.c_str());
            oldfile.write(year.c_str(), length5);
            oldfile.write(&delimiter, 1);
            int length6 = strlen(price.c_str());
            oldfile.write(price.c_str(), length6);
            oldfile.write(&delimiter, 1);
            int currRecSize = length1 + length2 + length3 + length4 + length5 + length6 + 6; /// 6 bytes of 6 delimiters.
            char recordCompleter = '-';
            if ( currRecSize < totalRecSize){
                for(int z = 0; z < (totalRecSize - currRecSize); z++){
                    oldfile.write((char*)&recordCompleter, 1);
                }
            }
        ID="";
        Title="";
        Author.clear();
        pages.clear();
        year.clear();
        price.clear();
    }
    oldfile.close();
    newfile.close();
    newfile.open(secondfile, ios::out|ios::trunc);
    newfile.close(); ///To empty newfile for future use.
    }

int main()
{
    Book b;
    fstream file;
    file.open(filename, ios::in|ios::out);
    short header = -1;

    file.seekp(0, ios::end);
    if(file.tellp() == 0){ ///If file is empty, write -1
    file.write((char*)&header, sizeof(header));}
    file.seekg(0, ios::beg);
    file.read((char*)&header, sizeof(header)); ///Read current header value into header variable
        file.close();
    int choice;
    while(choice!=0)
    {
        cout << "Enter a number or 0 to exit\n" <<"~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
        cout << "-1 to add a book\n\n" << "-2 to search a book\n\n" << "-3 to delete a book\n\n"
        << "-4 to print a book\n\n"  << "-5 to print all books\n\n" << "-6 to update a book\n\n" << "-7 to compress the file\n\n";
        cin >> choice;
        switch(choice)
        {
        case 0:
            cout << "Goodbye\n";
            break;
        case 1:
            addBook(header);
            continue;
        case 2:
            cout << "Input the ISBN of the book you want to search\n\n";
            cin >> b.isbn;
            Search(b.isbn);
            continue;
        case 3:
            Delete(header);
            continue;
        case 4:
            cout << "Input the ISBN of the book you want to print\n\n";
            cin >> b.isbn;
            Print(b.isbn);
            continue;
        case 5:
            PrintAll();
            continue;
        case 6:
            cout << "Input the ISBN of the book you want to update\n\n";
            cin >> b.isbn;
            update(b.isbn);
            continue;
        case 7:
            CompressFile(header);
            continue;
        }
    }
}


