#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;
struct flat
{
    string id;
    int initial_bandwidth;
    bool is_empty = false ; 
    flat *next = NULL ;
    flat *prev = NULL;
};

struct apartment{
    string name;
    int max_bandwidth;
    apartment *next = NULL ;
    flat *flatpointer = NULL ;
    
};
apartment *head ; // I keepng head of circular linked list there.

void add_apartment(vector<string> commandList){
    apartment *apart = new apartment ;
    apart->name = commandList[1];
    stringstream ss(commandList[3]);int x;ss >> x;
    apart->max_bandwidth = x;
    if (commandList[2]=="head")
    {
        head = apart;
        head->next = head;
    }
    else{
        string afterOrBefore;
        istringstream is(commandList[2]);
        vector<string> commands;
        apartment *temp = head;
        while(getline(is, afterOrBefore, '_')) {
            commands.push_back(afterOrBefore);
        }
        if (commands[0] == "after")
        {
            while (temp->name!=commands[1])
            {
                temp = temp->next;
            }
            apart->next = temp->next;
            temp->next = apart;
        } 
        else if (commands[0] == "before")
        {
            if (head->name==commands[1])
            {
                while (temp->next!=head)
                {
                    temp = temp->next;
                }
                temp->next = apart;
                apart->next = head;
                apartment *temporary = new apartment;
                temporary = head;
                head = apart;
                apart->next = temporary;
            }
            else{
                while (temp->next->name!=commands[1])
                {
                    temp = temp->next;
                }
                apart->next = temp->next;
                temp->next = apart;           
            }
        }
    }
}

void add_flat(vector<string> commandList){
    flat *flatt = new flat;
    flatt->id = commandList[4];
    stringstream ss(commandList[3]);int x;ss >> x;
    apartment *tempApartment = head;
    while (tempApartment->name != commandList[1])
    {
        tempApartment = tempApartment->next;
    }
    int netBandwidth = 0;
    flat *width = tempApartment->flatpointer;
    
    if (width !=NULL)//Calculating bandwidth for new flat if new flat's bandwidth bigger than other flat's bandwidth, initalize appropriate value
    {
        netBandwidth+=width->initial_bandwidth;
        while (width->next!=NULL)
        {
            width = width->next;
            netBandwidth+=width->initial_bandwidth;
        }

        if (tempApartment->max_bandwidth>netBandwidth+x)
        {
            flatt->initial_bandwidth = x;
        }
        else
        {
            flatt->initial_bandwidth=tempApartment->max_bandwidth-netBandwidth;
        }
    }
    else
    {
        if (tempApartment->max_bandwidth<x)
        {
            flatt->initial_bandwidth = tempApartment->max_bandwidth;
        }
        else{    
            flatt->initial_bandwidth = x;
        }
    }

    if (tempApartment->flatpointer == NULL)
    {
        tempApartment->flatpointer = flatt;
    }
    else{
        stringstream sss(commandList[2]);int index;sss >> index;
        flat *tempflat1 = new flat;
        if (index == 0)
        {
            tempflat1 = tempApartment->flatpointer;
            tempApartment->flatpointer = flatt;
            flatt->next = tempflat1;
            tempflat1->prev = flatt;
        }
        else{
            tempflat1 = tempApartment->flatpointer;
            while (index != 1)
            {
                tempflat1 = tempflat1->next;
                index--;
            }
            if (tempflat1->next!=NULL)
            {
                flat *temporaryFlat = tempflat1->next;
                tempflat1->next = flatt;
                flatt->prev = tempflat1;
                flatt->next = temporaryFlat;
                temporaryFlat->prev = flatt;
            }
            else
            {
                tempflat1->next = flatt;
                flatt->prev = tempflat1;
            }
        }
    }
}

apartment * remove_apartment(vector<string> commandList){
    if (head->name == commandList[1]&&head->next==head)    // if there is a one apartment
    {
        flat *temp = head->flatpointer;
        while (temp!=NULL)
        {
            temp = temp->next;
            delete temp->prev;
        }

        
        return NULL;
    }
    else if(head->name == commandList[1])                   // if we want to remove first apartment
    {
        apartment *tempApart = head;
        while (tempApart->next!=tempApart)
        {
            tempApart = tempApart->next;
        }
        tempApart->next = head->next;
        flat *temp = head->flatpointer;
        while (temp!=NULL)
        {
            temp = temp->next;
            delete temp->prev;
        }
        delete head;
        return tempApart->next;
    }
    else                                                   // if we want to remove apartment from inside of list
    {
        apartment *tempApart1 = head;
        while (tempApart1->next->name!=commandList[1])
        {
            tempApart1 = tempApart1->next;
        }
        apartment *removedApartment = tempApart1->next;
        tempApart1->next = removedApartment->next;
        flat *temp = removedApartment->flatpointer;
        while (temp!=NULL)
        {
            temp = temp->next;
            delete temp->prev;
        }
        delete removedApartment;
        return head;
    }
}
void make_flat_empty(vector<string> commandList){ // find true apartment and flat after that 

    apartment * tempApartment = head;

    while (tempApartment->name!=commandList[1])
    {
        tempApartment = tempApartment->next;
    }

    flat * tempflat1 = tempApartment->flatpointer;

    while (tempflat1->id!=commandList[2])
    {
        tempflat1 = tempflat1->next;
    }

    tempflat1->initial_bandwidth = 0;
    tempflat1->is_empty = true;

}

int find_sum_of_max_bandwidth(){
    if (head==NULL)
    {
        return 0;
    }
    else
    {
        apartment * tempApartment = head;
        int sum = 0;
        while (tempApartment->next!=head)
        {
            sum += tempApartment->max_bandwidth;
            tempApartment = tempApartment->next;
        }
        sum += tempApartment->max_bandwidth;
        return sum;
    }
}
apartment * merge_two_apartments(vector<string> commandList){
    apartment *apart1 ;
    apartment *apart2 ;
    apartment *iter1 = head;
    apartment *iter2 = head;
    if (iter1->name==commandList[1])
    {
        apart1 = iter1;
    }
    else
    {
        while (iter1->next!=head)
        {
            if (iter1->next->name==commandList[1])
            {
                apart1 = iter1->next;
                break;
            }
            iter1 = iter1->next;
        }
        
    }
    if (iter2->name== commandList[2])
    {
        apart2 = iter2;
    }
    else
    {   
        while (iter2->next!=head)
        {
            if (iter2->next->name==commandList[2])
            {
                apart2 = iter2->next;
                iter2->next = apart2->next;
                break;
            }
            iter2 = iter2->next;
        }
    }

    if (apart1->flatpointer==NULL&&apart2->flatpointer!=NULL)
    {
        apart1->flatpointer == apart2->flatpointer;
    }
    else if (apart1->flatpointer!=NULL&&apart2->flatpointer!=NULL)
    {
        flat *iter = apart1->flatpointer;
        while (iter->next!=NULL)
        {
            iter = iter->next;
        }
        iter->next = apart2->flatpointer;
        apart2->flatpointer->prev = iter;
    }
    apart1->max_bandwidth+= apart2->max_bandwidth;

    if (apart2==head)
    {
        apartment *iter3 = head;
        while (iter3->next!=head)
        {
            iter3 = iter3->next;
        }
        iter3->next = iter3->next->next;
        return iter3->next;
    }
    else
    {
        return head;
    }

}
void list_apartments(string out)
{
    ofstream outputfile;
    outputfile.open(out.c_str(),ios::app);
    if (head!=NULL)
    {
        
    apartment *temp1 = head;
    outputfile << temp1->name << "(" << temp1->max_bandwidth <<")\t";
    flat * tempFlatt = temp1->flatpointer;
    if (tempFlatt!=NULL)
    {
        while (tempFlatt->next!=NULL)
    {
        outputfile << "Flat" << tempFlatt->id << "("<< tempFlatt->initial_bandwidth << ")\t" ;
        tempFlatt = tempFlatt->next;
    }
        outputfile << "Flat" << tempFlatt->id << "("<< tempFlatt->initial_bandwidth << ")\t" ;
        outputfile << endl;
    } 
    else{
        outputfile << endl;
    }

    temp1 = temp1->next;
    while (temp1!= head)
    {
        outputfile << temp1->name << "(" << temp1->max_bandwidth <<")\t" ;
        if (temp1->flatpointer!=NULL)
        {
            flat * tempFlattt = temp1->flatpointer;

            while (tempFlattt->next!=NULL)
            {
                outputfile << "Flat" << tempFlattt->id << "("<< tempFlattt->initial_bandwidth << ")\t" ;
                tempFlattt = tempFlattt->next;
            }
            outputfile << "Flat" << tempFlattt->id << "("<< tempFlattt->initial_bandwidth << ")\t" ;
            outputfile << endl;
        }
        else
        {
            outputfile << endl;
        }
        
        temp1 = temp1->next;
    }
    }
    else
    {
        outputfile << "There is no apartment" << endl;
    }
    outputfile << endl;
}
void relocate_flats_to_same_apartment(vector<string> commands)
{
    apartment *temp = head;
    apartment *apart;

    while (temp->next!=head)
    {
        if (temp->name==commands[1])
        {
            apart = temp;
            break;
        }
        temp = temp->next;
    }
    if (temp->name==commands[1])
    {
        apart = temp;
    }
    flat *tempFlat = apart->flatpointer;

    while (tempFlat->id!=commands[2])
    {
        tempFlat = tempFlat->next;
    }
    string flats = commands[3];

    string flatss = flats.substr(1,flats.size()-2);
    
    vector<string> relocatedFlats;

    stringstream s(flatss);
    string ss;

    while (getline(s,ss,','))
    {
        stringstream str(ss);
        string stri;
        str >> stri;
        relocatedFlats.push_back(stri);
    }

    for (int i = 0; i < relocatedFlats.size(); i++)
    {

        string flatId = relocatedFlats[i];
        apartment *tempp = head;

        while (tempp->next!=head)
        {

            flat *tempflt = tempp->flatpointer;
            while (tempflt->id!=flatId&&tempflt->next!=NULL)
            {

                tempflt = tempflt->next;
            }
            if(tempflt->id==flatId){
                flat *temporar = tempflt->prev;
                if (tempflt->next== NULL)
                {
                    temporar->next = NULL;
                    
                }
                else
                {
                    temporar->next = tempflt->next;
                    temporar->next->prev = temporar;
                }
                tempp->max_bandwidth-=tempflt->initial_bandwidth;

                if (apart->flatpointer==tempFlat)
                {
                    apart->flatpointer = tempflt;
                    tempflt->next = tempFlat;
                    tempFlat->prev = tempflt;
                    
                }
                else
                {
                    tempFlat->prev->next = tempflt;
                    tempflt->prev = tempFlat->prev;
                    tempflt->next = tempFlat;
                    tempFlat->prev = tempflt;
                }
                apart->max_bandwidth+=tempflt->initial_bandwidth;
                break;
            }

            tempp = tempp->next;
        }
        if (tempp->name==commands[1])
        {

            flat *tempflt = tempp->flatpointer;
            while (tempflt->id!=flatId&&tempFlat->next!=NULL)
            {
                tempflt = tempflt->next;
            }
            if(tempflt->id==flatId){
                tempflt->prev->next = tempflt->next;
                tempFlat->prev = tempflt;
                tempflt->next = tempFlat;
            }    
        }
    } 
}



void fileOperation(string file1,string output){
    ifstream file;
    file.open(file1.c_str());
    string line;
    
    while(getline(file, line)) {     // '\n' is the default delimiter
        vector<string> tokens;
        istringstream iss(line);
        string token;
        while(getline(iss, token, '\t')) {
            tokens.push_back(token);
        }                                                                        // but we can specify a different one
        if (tokens[0] == "add_apartment")
        {
            add_apartment(tokens);     
        }
        else if (tokens[0] == "add_flat")
        {
            add_flat(tokens);
        }
        else if (tokens[0] == "remove_apartment")
        {
            head = remove_apartment(tokens);
        }
        else if (tokens[0] == "merge_two_apartments")
        {
            head = merge_two_apartments(tokens);
        }
        else if(tokens[0] == "find_sum_of_max_bandwidths"){
            int sum = 0;
            sum = find_sum_of_max_bandwidth();
            ofstream outputfile;
            outputfile.open(output.c_str(),ios::app);
            outputfile << "sum of bandwidth: " << sum << endl;
            outputfile << endl;
            outputfile.close();
        }
        else if(tokens[0] == "list_apartments"){
            list_apartments(output);
        }
        else if (tokens[0] == "make_flat_empty")
        {
            make_flat_empty(tokens);
        }
        else if (tokens[0] == "relocate_flats_to_same_apartment")
        {
            relocate_flats_to_same_apartment(tokens);
        }
    }
    file.close();
}

int main(int argc, char* argv[]){ 
    string file = argv[1];
    string output = argv[2];
    fileOperation(file,output);
    
    return 0;
}