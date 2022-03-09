#include <iostream>
#include <stdio.h>

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


	void add(int key, Student* value){
		Pair* p = new Pair();
		p->key = key;
		p->value = value;

		Node* n = new Node();
		n->data = p;
		n->next = head;
		this->head = n;
	}


};




class Hashmap{
private:

Bucket* buckets[128];
int num_buckets = 128;
	
	int hash(char* data){
		int sum = 0;
		for(int i = 0, len=strlen(data); i < len; i++){
			sum += data[i];

		}
		return sum;
	}
	
public:
	Hashmap(){
		for(int i = 0; i < this->num_buckets; i++){
			this->buckets[i] = new Bucket();
		}
	}

	void add(Pair* p){
		int bucket = p->key % num_buckets;
		buckets[bucket]->add(p->key, p->value);
	}

	void add(int key, Student* value){
		Pair* p = new Pair();
		p->key = key;
		p->value = value;
		this->add(p);
	}

	Student* get(int key){
		int bucket = key % num_buckets;
		Pair* p = buckets[bucket]->get(key);
		return p->value;
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
				//std::cout << "(" << curr->data->key << "=" << curr->data->value << ")->";
				curr->data->value->print();
				std::cout << "->";
				curr = curr->next;
			}
			std::cout << "\n";
		}

	}

};



int main(){


	
	Hashmap* h = new Hashmap();
	
	Student* s = new Student();
	h->add(s->student_id, s);

	Student* s1 = new Student();
	h->add(s1->student_id, s1);

	//std::cout << h->get(1) << "\n";
	//std::cout << h->get(2) << "\n";
	h->viz();
}
