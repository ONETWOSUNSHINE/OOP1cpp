﻿#include <string>
#include <iostream>
template<typename T>
class NewContainer {
public:
	class Iterator;
	friend class Iterator;

private:
	class Element;
	friend class Element;

	class Element
	{
	public:
		friend class NewContainer<T>;
		friend class Iterator;
		Element(T elem) : value(elem) {}
		Element() : value(0) {}
		~Element() {}
		void print_value()
		{
			std::cout << value << " ";
		}
		Element* next = nullptr;
		Element* prev = nullptr;
		T value;
	};

public:
	class Iterator
	{
		friend class NewContainer;
	public:
		Iterator() :node(nullptr) {}
		Iterator(Element* elem) :node(elem) {}
		Iterator(const Iterator& other) :node(other.node) {}
		Iterator& operator=(const Iterator& other)
		{
			node = other.node;
			return *this;
		}
		bool operator==(const Iterator& other) const
		{
			return (node == other.node);
		}
		bool operator!=(const Iterator& other) const
		{
			return (node != other.node);
		}
		Iterator& operator++()
		{
			if (node == nullptr)
				throw "incremented an empty iterator";
			if (node->next == nullptr)
				throw "tried to increment too far past the end";
			node = node->next;
			return *this;
		}
		Iterator& operator--()
		{
			if (node == nullptr)
				throw "incremented an empty iterator";
			if (node->prev == nullptr)
				throw "tried to increment too far past the end";
			node = node->prev;
			return *this;
		}
		T& operator*() const
		{
			if (node == nullptr)
				throw "tried to dereference an empty iterator";
			return node->value;
		}
	private:
		Element* node;
	};




private:
	Element* head;
	Element* afterTail;
	Iterator headIter;
	Iterator tailIter;






public:
	NewContainer()
	{
		head = afterTail = new Element;
		head->next = nullptr;
		afterTail->prev = nullptr;
		headIter = Iterator(head);
		tailIter = Iterator(afterTail);
	}
	NewContainer(T node)
	{
		head = afterTail = new Element;
		head->next = nullptr;
		afterTail->prev = nullptr;
		headIter = Iterator(head);
		tailIter = Iterator(afterTail);
		add_front(node);
	}
	~NewContainer()
	{
		Element* node_to_delete = head;
		for (Element* sn = head; sn != afterTail;)
		{
			sn = sn->next;
			delete node_to_delete;
			node_to_delete = sn;
		}
		delete node_to_delete;
	}
	bool is_empty()
	{
		return head == afterTail;
	}
	Iterator front()
	{
		return headIter;
	}
	Iterator back()
	{
		return tailIter;
	}
	void add_front(T value)
	{
		Element* add = new Element(value);
		add->next = head;
		add->prev = nullptr;
		head->prev = add;
		head = add;
		headIter = Iterator(head);
	}
	void add_back(T value)
	{
		if (is_empty())
			add_front(value);
		else
		{
			Element* add = new Element(value);
			add->next = afterTail;
			add->prev = afterTail->prev;
			afterTail->prev->next = add;
			afterTail->prev = add;
			tailIter = Iterator(afterTail);
		}

	}
	void print() const
	{
		for (Element* dn = head; dn != afterTail; dn = dn->next)
		{
			dn->print_value();
		}

		std::cout << std::endl;
	}
	T remove_front()
	{
		if (is_empty())
		{
			throw "tried to remove from an empty list";
		}
		Element* remove = head;
		T return_val = remove->value;
		head = remove->next;
		head->prev = nullptr;
		headIter = Iterator(head);
		delete remove;
		return return_val;

	}
	T remove_back()
	{
		if (is_empty())
		{
			throw "tried to remove from an empty list";
		}
		Element* remove = afterTail->prev;
		if (remove->prev == 0)
		{
			return remove_front();
		}
		else
		{
			T return_val = remove->value;
			remove->prev->next = afterTail;
			afterTail->prev = remove->prev;
			delete remove;
			return return_val;
		}

	}
	T first() const
	{
		return  head->value;
	}
	T last() const
	{
		return afterTail->prev->value;
	}
	int size() const
	{
		int s = 0;
		for (Element* i = head; i != afterTail; i = i->next)
		{
			s++;
		}
		return s;
	}
	void swap(NewContainer& f)
	{
		Element* r = f.head;
		for (Element* i = head; i != afterTail; i = i->next)
		{
			T sw = i->value;
			i->value = r->value;
			r->value = sw;
			r = r->next;
		}

	}
	void reverse()
	{
		Element* f = head;
		Element* l = afterTail->prev;
		if (size() % 2 == 0)
		{
			int s = 0;
			while (size() / 2 != s)
			{
				T sw = f->value;
				f->value = l->value;
				l->value = sw;
				f = f->next;
				l = l->prev;
				s++;
			}
		}
		else
		{
			while (f != l)
			{
				T sw = f->value;
				f->value = l->value;
				l->value = sw;
				f = f->next;
				l = l->prev;
			}
		}
	}
};

class Object
{
public:
    virtual std::string toString() const = 0;
};

template<typename T>
class Task :public Object
{
    Task(T elem) : value(elem) {}
    Task() : value(0) {}
    ~Task() {}
    class binaryTask;
    class addTask;
    class amountTask;
    class namelessTask;
    class successorTask;
    T value;
};

class Named :public Object
{

};
template<typename T>
class binaryTask : public Task<double>
{
public:
    binaryTask(double a, double b): first(a), second(b) {}
    binaryTask() : first(0), second(0) {}
    double first;
    double second;
};
/*class additionTask : public binaryTask
{
public:
    additionTask(double a, double b) : first(a), second(b) {}
    std::string toString() const override
    {
        return std::to_string(first + second);

    }
    double first;
    double second;
};
class multiplicationTask : public binaryTask
{
public:
    multiplicationTask(double a, double b) : first(a), second(b) {}
    std::string toString() const override
    {
        return std::to_string(first * second);

    }
    double first;
    double second;
};
class divisionTask : public binaryTask
{
public:
    divisionTask(double a, double b) : first(a), second(b) {}
    std::string toString() const override
    {
        return std::to_string(first / second);

    }
    double first;
    double second;
};
class subtractionTask : public binaryTask
{
public:
    subtractionTask(double a, double b) : first(a), second(b) {}
    std::string toString() const override
    {
        return std::to_string(first - second);

    }
    double first;
    double second;
};*/


template<typename T>
class addTask : public Task<T>
{
public:
    addTask(T value, NewContainer<T> a) : first(value), Cont(a){}
	std::string toString() const override
	{
		Cont.add_back(first);

	}
	T first;
	NewContainer<T> Cont;
};



int main()
{
	NewContainer<double> s;
	for (int i = 2; i <= 7; ++i)
	{
		s.add_back(i);
	}
	s.print();
	double b = 5;
	addTask<double> a(b, s);




    //multiplicationTask a(5,4);
    //std::cout << a.toString();
}
