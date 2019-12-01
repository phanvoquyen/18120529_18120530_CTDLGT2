#pragma once
#include <iostream>
#include <queue>
using namespace std;

template<class T>

// Định nghĩa node trên cây huffman
struct HuffmanTreeNode
{
	T t; // trọng số của cây huffman
	HuffmanTreeNode<T>* left; // một node con trỏ huffman có kiểu dữ liệu T
	HuffmanTreeNode<T>* right;
	HuffmanTreeNode<T>* parent;

	// hàm dựng cho cấu trúc HuffmanTreeNode
	HuffmanTreeNode(const T &t = T()) :this->t(t), left(nullptr), right(nullptr), parent(nullptr)
	{}
};

template<class T> // định nghĩa lại template class này vì mỗi lần khai báo chỉ sử dụng đucợ cho một khối lệnh dưới nó

// Xây dựng lớp cây huffman
class HuffmanTree
{
	typedef HuffmanTreeNode<T> Node; // định nghĩa lại tên cho một node huffman kiểu dữ liệu T thành Node, mục đích cho ngắn gọn thôi
private:
	Node* root; // node gốc cây huffman
public:
	HuffmanTree() :root(nullptr)
	{
		cout << "HuffmanTree() is executing..." << endl;
	}

	void DestroyNode(Node* root)
	{
		if (root == nullptr)
			return;
		if (root->left)
			DestroyNode(root->left);
		if (root->right)
			DestroyNode(root->right);

		delete root;
	}

	// Để thuận tiện cho việc xây dựng cây huffman, ta xây dựng
	// một cấu trúc hàng đợi có ưu tiên vì mỗi lần xây dựng cây
	// ta cần chọn ra được hai node có trọng số nhỏ nhất, do đó
	// ta chọn cấu trúc đống, tức là cấu trúc có phần tử nhỏ nhất
	// nằm trên đỉnh, sử dụng priority queue tự định nghĩa để có 
	// phần tử nhỏ nhất luôn nằm ở đầu hàng đợi

	// muốn sử dụng priority queue ta phải tự xây dựng toán tử luận lí
	// nguồn: https://vi.wikipedia.org/wiki/M%C3%A3_h%C3%B3a_Huffman
	// nguồn: https://motoo.in/tutorial-su-dung-thu-vien-stl-trong-c-priority-queue/
	struct CompareNode
	{
		bool operator()(Node* l, Node* r)
		{
			return l->t > r->t;
		}
	};

	// Xây dựng cây huffman dựa vào kiểu priority queue
	HuffmanTree(T *t, size_t size, const T & illegal) // sử dụng mảng dữ liệu T để xây dựng trọng số cây huffman
	{
		cout << "HuffmanTree(T *t, size_t size, const T & illegal) is excuting..." << endl;
		priority_queue<Node*, vector<Node*>, CompareNode> minheap;

		// Lưu các kí tự trong đống vừa tạo để xây dựng cây huffman
		for (size_t i = 0; i < size; i++)
		{
			if (w[i] != illegal)
			{
				minheap.push(new Node(w[i]));
			}
		}

		// Xây dựng câu huffman theo đúng thuật toán
		while (minheap.size() > 1)
		{
			// lấy hai trọng số nhỏ nhất, cộng dồn tạo thành một node huffman
			Node* lefttmp = minheap.top(); // truy xuất phần tử ở đỉnh queue, là phần tử có trọng số bé nhất
			minheap.pop(); // sau khi lấy loại ngay phần tử ở đỉnh
			Node* righttmp = minheap.top();
			minheap.pop();
			Node* parenttmp = new Node(lefttmp->t + righttpm->t);
			// tạo quan hệ cây
			parent->left = lefttmp;
			parent->right = righttpm;
			lefttmp->parent = parenttmp;
			righttmp->parent = parenttmp;

			// sau đó thêm lại parenttmp vào minheap, tiếp tục quá trình xây cây huffman
			minheap.push(parenttmp);
		}

		// sau khi hoàn thành câu huffman, hiển nhiên phần tử ở đỉnh đống sẽ là root
		root = minheap.top();
		minheap.pop();
	}

	Node* getRoot() { return root; }

	~HuffmanTree()
	{
		DestroyNode(root);
		cout << "~HuffmanTree() is excuting..." << endl;
	}
};