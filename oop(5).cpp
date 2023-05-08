#include <iostream>
#include <string>
#include <random>
#include <cstdlib>
#include <ctime>
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
		Element() {}
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




public:
	Element* head;
	Element* afterTail;

public:
	NewContainer()
	{
		head = afterTail = new Element;
		head->next = nullptr;
		afterTail->prev = nullptr;
	}
	NewContainer(T node)
	{
		head = afterTail = new Element;
		head->next = nullptr;
		afterTail->prev = nullptr;
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
	void add_front(T value)
	{
		Element* add = new Element(value);
		add->next = head;
		add->prev = nullptr;
		head->prev = add;
		head = add;
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
	void remove_front()
	{
		if (is_empty())
		{
			throw "tried to remove from an empty list";
		}
		Element* remove = head;
		head = remove->next;
		head->prev = nullptr;
		delete remove;

	}
	void remove_back()
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
			remove->prev->next = afterTail;
			afterTail->prev = remove->prev;
			delete remove;
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
		Element* thisHead = head;
		head = f.head;
		f.head = thisHead;
		Element* thisafterTail = afterTail;
		afterTail = f.afterTail;
		f.afterTail = thisafterTail;

	}
	void reverse()
	{
		Element* currNode = head;
		Element* prevNode = nullptr;
		Element* nextNode = nullptr;
		int n = size();
		for (int i = 1; i != n + 1; i++)
		{
			nextNode = currNode->next;
			prevNode = currNode->prev;
			currNode->next = prevNode;
			currNode->prev = nextNode;
			prevNode = currNode;
			currNode = nextNode;
		}
		afterTail = head->next;
		head = prevNode;
	}
	void removeAll() {
		Element* current = head;
		while (current != nullptr) {
			Element* temp = current;
			current = current->next;
			delete temp;
		}
		head = nullptr;
		afterTail = nullptr;
	}
};


class Object {
public:
	Object()
	{
		++count;
	}

	virtual ~Object()
	{
		--count;
	}

	virtual std::string toString() const = 0;
	static int count; // счетчик объектов
};

int Object::count = 0;

class Task : public Object {
public:
	virtual bool execute() = 0;
};

class ResultTask : public Task {
public:
	virtual double getResult() const = 0;
};

class Named : public Object {
protected:
	std::string name;

public:
	Named(const std::string& n) : name(n) {}

	virtual std::string toString() const override {
		return "Named object: " + name;
	}
};

class BinaryArithmeticTask : public Named, public ResultTask {
public:
	enum Operation { Addition, Multiplication, Division, Subtraction };
	BinaryArithmeticTask(const std::string& n, double op1, double op2, char op): Named(n), operand1(op1), operand2(op2), operation(op) {}
	virtual bool execute() override {
		switch (operation) {
		case Addition:
			result = operand1 + operand2;
			break;
		case Multiplication:
			result = operand1 * operand2;
			break;
		case Division:
			if (operand2 == 0)
			{
				throw "Invalid operation";
			}
			result = operand1 / operand2;
			break;
		case Subtraction:
			result = operand1 - operand2;
			break;
		default:
			return false; // некорректный оператор
		}
		return true;
	}

	virtual double getResult() const override
	{
		return result;
	}

	virtual std::string toString() const override {
		return "Binary arithmetic task: " + name + " (" + std::to_string(operand1) + " " + "," + " "
			+ std::to_string(operand2) + ") = " + std::to_string(getResult());
	}

private:
	double operand1;
	double operand2;
	double result;
	char operation;

};

class AddTask : public Task {
public:
	AddTask(Task* t, NewContainer<Task*>& tasks) : task(t), taskList(tasks) {}

	virtual bool execute() override {
		taskList.add_back(task);
		return true;
	}

	virtual std::string toString() const override {
		return "Add task: " + task->toString() + " to task list";
	}

private:
	Task* task;
	NewContainer<Task*>& taskList;

};

class CountObjectsTask : public ResultTask {
public:
	CountObjectsTask(const NewContainer<Task*>& objs) : objects(objs) { }

	virtual bool execute() override {
		result = objects.size();
		return true;
	}

	virtual double getResult() const override
	{
		return int(result);
	}

	virtual std::string toString() const override {
		return "Count objects task in cont: " + std::to_string(getResult());
	}

private:
	const NewContainer<Task*>& objects;
	int result;

};

class CountResultTasksTask : public ResultTask {
public:
	CountResultTasksTask(const NewContainer<Task*>& tasks) : taskList(tasks) {}

	virtual bool execute() override {
		int resultTaskCount = 0;
		for (auto task = taskList.head; task != taskList.afterTail; task = task->next)
		{
			if (dynamic_cast<ResultTask*>(task->value) != nullptr)
			{
				resultTaskCount++;
			}
		}
		return true;
	}

	virtual double getResult() const override {
		int resultTaskCount = 0;
		for (auto task = taskList.head; task != taskList.afterTail; task = task->next)
		{
			if (dynamic_cast<ResultTask*>(task->value) != nullptr) {
				resultTaskCount++;
			}
		}
		return int(resultTaskCount);
	}

	virtual std::string toString() const override {
		return "Count result tasks task: " + std::to_string(getResult());
	}

private:
	const NewContainer<Task*>& taskList;

};

class ClearObjectsTask : public Task { // По какой-то причине он удаляет только объекты из списка но сами объекты остаются
public:
	ClearObjectsTask(NewContainer<Task*>& objs) : objects(objs) {}

	  virtual bool execute() override
	  {
		  objects.removeAll();
		  return true;
	  }

	  virtual std::string toString() const override {
		  return "Clear objects task completed";
	  }

private:
	NewContainer<Task*>& objects;
};

class TotalObjectCountTask : public ResultTask {
public:
	TotalObjectCountTask(){}
	virtual bool execute() override
	{
		result = Object::count;
		return true;
	}
	virtual double getResult() const override 
	{
		return int(result);
	}
	virtual std::string toString() const override
	{
		return "Total object count task: " + std::to_string(getResult());
	}

private:
	int result;
};

enum Operation { Addition, Multiplication, Division, Subtraction };
Operation getRandomOperation() {
	int op = rand() % 4;
	switch (op) {
	case 0:
		return Addition;
	case 1:
		return Multiplication;
	case 2:
		return Division;
	case 3:
		return Subtraction;
	default:
		std::cerr << "Unknown operation" << std::endl;
		exit(1);
	}
}
std::string OperationToString(Operation op) {
	switch (op) {
	case Addition:
		return "Addition";
	case Multiplication:
		return "Multiplication";
	case Division:
		return "Division";
	case Subtraction:
		return "Subtraction";
	default:
		return "UNKNOWN";
	}
}

int main()
{
	std::cout << "Total object count: " << Object::count << std::endl;
	NewContainer<std::string> info;
	NewContainer<Task*> tasks;
	std::cout << "Total object count: " << Object::count << std::endl;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<double> dis(1, 100);
	for (int i = 0; i < 5; ++i) 
	{
		Operation op = getRandomOperation();
		std::string opStr = OperationToString(op);
		double a = dis(gen);
		double b = dis(gen);
		BinaryArithmeticTask* binarytask = new BinaryArithmeticTask(opStr, a, b, op);
		AddTask* addTask = new AddTask(binarytask, tasks);
		addTask->execute();
		delete addTask;
	}

	CountResultTasksTask* countResultTasksTask = new CountResultTasksTask(tasks);
	AddTask* addTask1 = new AddTask(countResultTasksTask, tasks);
	addTask1->execute();
	TotalObjectCountTask* totalObjectCountTask = new TotalObjectCountTask();
	AddTask* addTask2 = new AddTask(totalObjectCountTask, tasks);
	addTask2->execute();
	CountObjectsTask* countObjectsTask = new CountObjectsTask(tasks);
	AddTask* addTask3 = new AddTask(countObjectsTask, tasks);
	addTask3->execute();

	std::cout <<"Total object count: "<< Object::count << std::endl;

	for (auto task = tasks.head; task != tasks.afterTail; task = task->next)
	{
		task->value->execute();
		info.add_back(task->value->toString());
		
	}
	std::cout << " " << std::endl;
	for (auto task = info.head; task != info.afterTail; task = task->next)
	{
		std::cout << task->value << std::endl;

	}

	ClearObjectsTask* clearObjectsTask = new ClearObjectsTask(tasks);
	clearObjectsTask->execute();
	tasks.print();
	//info.removeAll();
	delete addTask1;
	delete addTask2;
	delete addTask3;
	delete clearObjectsTask;
	std::cout << "Total object count: " << Object::count << std::endl;
}
