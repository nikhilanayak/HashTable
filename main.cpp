#include <iostream>
#include <fstream>
#include <limits>
#include <algorithm>
#include <stdio.h>
#include <cstdlib> 
#include <ctime>

template<typename T>
T get_type(){ // gets input of a type, ignoring all other types
	T input;
	while(!(std::cin >> input)){
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // clear the whole buffer
		std::cout << "Invalid input, please try again: ";
	}
	return input;
}



class Student{
public:
char first_name[256];
char last_name[256];
int student_id;
float GPA;
public:
	Student(){
		std::cout << "Input a first name: ";
		std::cin >> this->first_name;

		std::cout << "Input a last name: ";
		std::cin >> this->last_name;

		std::cout << "Input a (unique) student id: ";
		this->student_id = get_type<int>();

		std::cout << "Input a GPA: ";
		this->GPA = get_type<float>();
	}

	Student(char* first_name, char* last_name, int student_id, float GPA){
		strcpy(this->first_name, first_name);
		strcpy(this->last_name, last_name);
		this->student_id = student_id;
		this->GPA = GPA;
	}

	void print(){
		printf("(%s %s: student id=%i, GPA=%.3f)", this->first_name, this->last_name, this->student_id, this->GPA);
	}
};


class Pair{
public:
	int key;
	Student* value;
	Pair(int key, Student* value){
		this->key = key;
		this->value = value;
	}
	Pair(){

	}
};

typedef struct Node{
	Pair* data;
	Node* next;
} Node;



class Bucket{
public:
Node* head = NULL;
int size = 0;
	Bucket(){

	}
	Pair* get(int key){
		Node* curr = this->head;
		while(curr != NULL){
			if(curr->data->key == key){
				return curr->data;
			}
			curr = curr->next;
		}
		return NULL;
	}

	void delete_(int key){
		Node* curr = this->head;
		Node* next = curr->next;

		if(next == NULL){
			delete this->head;
			this->head = NULL;
			this->size = 0;
			return;
		}

		while(next != NULL){
			if(next->data->key == key){
				curr->next = next->next;
				this->size--;
				return;
			}
			curr = curr->next;
			next = next->next;
		}
	}



	void add(int key, Student* value){
		Pair* p = new Pair();
		p->key = key;
		p->value = value;

		Node* n = new Node();
		n->data = p;
		n->next = head;
		this->head = n;
		this->size += 1;
	}
};




class Hashmap{
private:

//Bucket* buckets[128];

	int hash(char* data){
		int sum = 0;
		for(int i = 0, len=strlen(data); i < len; i++){
			sum += data[i];

		}
		return sum;
	}
	
public:
Bucket** buckets;
int num_buckets;
	Hashmap(int num_buckets){
		this->num_buckets = num_buckets;
		buckets = new Bucket*[this->num_buckets];

		for(int i = 0; i < this->num_buckets; i++){
			this->buckets[i] = new Bucket();
		}
	}

	int add(Pair* p){
		int bucket = p->key % num_buckets;
		buckets[bucket]->add(p->key, p->value);
		return bucket;
	}

	int add(int key, Student* value){
		Pair* p = new Pair();
		p->key = key;
		p->value = value;
		return this->add(p);
	}

	Student* get(int key){
		int bucket = key % num_buckets;
		Pair* p = buckets[bucket]->get(key);
		return p->value;
	}

	void delete_(int key){
		int bucket = key % num_buckets;
		buckets[bucket]->delete_(key);
	}

	bool exists(int key){
		int bucket = key % num_buckets;
		Pair* p = buckets[bucket]->get(key);
		return p != NULL;
	}

	void viz(){
		for(int i = 0; i < num_buckets; i++){
			if (buckets[i]->head == NULL) continue;
			std::cout << "bucket " << i << ": ";
			Node* curr = buckets[i]->head;
			while(curr != NULL){
				curr->data->value->print();
				std::cout << "->";
				curr = curr->next;
			}
			std::cout << "\n";
		}

	}

	Hashmap* double_buckets(){
		Hashmap* newhm = new Hashmap(this->num_buckets * 2);

		for(int i = 0; i < this->num_buckets; i++){
			Node* head = this->buckets[i]->head;
			while(head != NULL){
				newhm->add(head->data);
				head = head->next;
			}

		}
		return newhm;
	}
};


char* get_first_name(){
	std::fstream fnames;
	fnames.open("FIRST_NAMES.txt", std::ios::in);
	int index = rand() % 100;
	char* out = new char[256];
	for(int i = 0; i < index; i++) fnames >> out;
	return out;
}
char* get_last_name(){
	std::fstream fnames;
	fnames.open("LAST_NAMES.txt", std::ios::in);
	int index = rand() % 100;
	char* out = new char[256];
	for(int i = 0; i < index; i++) fnames >> out;
	return out;
}


int main(){
	srand((unsigned)time(0)); 


	Hashmap* h = new Hashmap(2);
	while(true){
		std::cout << "A(dd) P(rint) D(elete) Q(uit), R(andom): ";
		char next_line[256];
		std::cin >> next_line;
		char input = next_line[0];
		switch(input){
			case 'A': {
				Student* s = new Student();
				int b = h->add(s->student_id, s);
				if(h->buckets[b]->size > 3){
					std::cout << "collision\n";
				}
				break;}
			case 'P':{
				for(int i = 0; i < h->num_buckets; i++){
					Node* curr = h->buckets[i]->head;
					while(curr != NULL){
						curr->data->value->print();
						std::cout << "\n";
						curr = curr->next;
					}

				}
				break;}
			case 'D':{
				std::cout << "Student id: ";
				int id = get_type<int>();
				h->delete_(id);
				break;}
			case 'Q':{
				exit(0);
				break;}
			case 'R': {
				std::cout << "How many: ";
				int how_many = get_type<int>();
				for(int i = 0; i < how_many; i++){
					char* first_name = get_first_name();
					char* last_name = get_last_name();
					int id = 0;
					while(h->exists(id)) id++;
					Student* s = new Student(first_name, last_name, id, rand() / (RAND_MAX + 1.0) * 4.0);
					h->add(id, s);
				}

				  break;}
		}


	}

}
