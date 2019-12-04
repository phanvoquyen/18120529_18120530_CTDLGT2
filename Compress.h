#pragma once
#include"HuffmanTree.h"
#include<string>
#include<fstream>
#include<iostream>
using namespace std;

struct HashInfo // cấu trúc dữ liệu trong hàm băm
{
	char ch;
	size_t count;
	string code;

	HashInfo() : ch('\0'), count(0), code("")
	{}
	HashInfo(const HashInfo& h)
	{
		this->ch = h.ch;
		this->count = h.count;
		this->code = h.code;
	}

	bool operator!=(const HashInfo& h)
	{
		return this->count != h.count;
	}
	void operator=(const HashInfo& h)
	{
		this->ch = h.ch;
		this->code = h.code;
		this->count = h.count;
	}
	bool operator<(const HashInfo& h)
	{
		return this->count < h.count;
	}
	bool operator>(const HashInfo& h)
	{
		return this->count > h.count;
	}

	HashInfo operator+(const HashInfo& h)
	{
		HashInfo tmp;
		tmp.count = this->count + h.count;
		tmp.code = this->code + h.code;
		return tmp;
	}
};

struct CharInfo
{
	char ch;
	size_t count;
	CharInfo() :ch('\0'), count(0)
	{}
};

class FileCompress
{
	typedef HuffmanTreeNode<HashInfo> Node;
private:
	HashInfo hashInfo[256]; // tạo một bảng băm 256 kí tự, dựa theo các kí tự ACSII
public:
	FileCompress() // khởi tạo bảng băm
	{
		for (size_t i = 0; i < 256; ++i)//khởi tạo nhóm băm, sử dụng bảng mã acsii làm bảng chuyển đổi code
		{
			hashInfo[i].ch = (unsigned char)i; // unsigned char để đổi về bảng 256 acsii
			hashInfo[i].count = 0;
		}
		cout << "FileCompress() is excuting..." << endl;
	}

	// hàm nén
	void Compress(const string& file)
	{
		char c;
		string filename(file);
		ifstream ifs(file, ios::binary); // mở file kiểu nhị phân

		// tiến hành chuyển đổi lần lượt các kí tự trong tệp, ép kiểu unsigned char cho đồng bộ với mã acsii
		while (ifs.get(c))
		{
			hashInfo[(unsigned char)c].count++;
		}

		HashInfo illegal;
		illegal.count = 0;

		// xây dựng cây huffman bằng cách tạo đống, hàm gọi bên HuffmanTree.h
		HuffmanTree<HashInfo> tree(hashInfo, 256, illegal);
		// dựa vào cây huffman mới tạo, xây dựng mã code mã hóa kí tự và lưu trữ mã hóa này vào hashInfo, code dưới dạng 0 và 1, theo nguyên tắc qua phải cây là 1, qua trái là 0
		MakeCode(tree.getRoot());

		// tạo một tệp mới để ghi dữ liệu nén
		cout << "Nhap ten file nen (ten cua file khi nen xong): " << endl;
		string newfilename;
		cin >> newfilename;
		newfilename += ".huffman";

		// tiến hành lặp qua các kí tự trong tệp nguồn cần nén, sau đó tạo tệp mới, ghi lại mã hóa tuân theo hashInfo
		ofstream ofs(newfilename, ios::binary);
		string suf = file.substr(file.rfind('.')); // substr chép chuỗi từ vị trí của '.' tới cuối, rfind tìm vị trí cuối cùng của '.' trong chuỗi
		size_t suf_count = suf.size();
		ofs.write((char*)&suf_count, sizeof(suf_count));
		ofs.write(suf.c_str(), suf.size()); // ghi lại tên của loại tệp được nén

		// tiến hành ghi mã hóa ra file sau khi đã ghi lại tên loại tệp đucợ nén
		// trước khi nén tiến hành ghi lại số lần mỗi kí tự xuất hiện
		// sử dụng để xây dụng cây huffman trước khi giải nén
		// dựa vào hashInfo, ghi chuỗi mã hóa vào file
		CharInfo chInfo;
		for (size_t i = 0; i < 256; i++)
		{
			if (hashInfo[i].count > 0)
			{
				chInfo.ch = hashInfo[i].ch; // gán trường _ch của hashInfo vào chInfo, hashInfo và chInfo là hai struct có các trường giống nhau, tuy nhiên hashInfo lưu giá trị băm nên thêm trường _code, trường này lưu mã mã hóa
				chInfo.count = hashInfo[i].count;
				ofs.write((char*)&chInfo, sizeof(CharInfo)); // chèn sizeof(CharInfo) kí tự từ (char*)&chInfo vào stream
			}
		}

		// tạo thêm một kí tự kết thúc để nhận biết khi nén file
		CharInfo end;
		end.count = 0;
		ofs.write((char*)&end, sizeof(CharInfo));// chèn sizeof(CharInfo) kí tự từ (char*)&end vào stream, phép (char*)&end được hiểu là lấy trường char của struct CharInfo, (char*)&end = &end._ch, &end._ch không in ra địa chỉ theo hệ hexa mà in ra bậy bậy, nếu ép kiểu về int thì nó in ra đúng địa chỉ của end
		// tới đây coi như đã chuyển đổi thành công dữ liệu

		// Bây giờ tiến hành đặt lại ví trí đọc tệp, để ifs trỏ về đầu tệp
		// tiến hành giải thuật nén huffman như trên tài liệu wiki
		// đọc lần lượt qua từng kí tự của file nguồn, sau đó dựa vào hashInfo.code
		// chuyển đổi ngược code thành các kí hiệu trong bảng acsii, code hiện tại đang là chuỗi nhị phân
		// cứ mỗi lần ghi đủ 8 bit thì chép vào file
		ifs.clear();
		ifs.seekg(0); // dời con trỏ về đầu
		char newch = '\0';
		int pos = 0; // độ dịch chuyển bit

		string tmpcode;
		while (ifs.get(c))
		{
			tmpcode = hashInfo[(unsigned char)c].code;

			for (size_t i = 0; i < tmpcode.size(); i++)
			{
				if (tmpcode[i] == '0')
				{
					newch = newch & (~(1 << pos));
				}
				else
				{
					newch = newch | (1 << pos);
				}
				pos++;

				if (pos == 8) // đủ 8 bit theo quy ước thuật toán
				{
					ofs.put(newch);
					pos = 0; // thiết lập lại pos
					newch = '\0'; // thiết lập lại newch
				}
			}
		}

		if (pos != 0)
		{
			ofs.put(newch);
		}


	}

	void MakeCode(Node* root)
	{
		if (root->left == nullptr && root->right == nullptr)
		{
			hashInfo[(unsigned char)root->t.ch].code = root->t.code; // root có kiểu HuffmanTree<HashInfo>, root->_w có kiểu hashInfo, _code là trường của hashinfo
			std::cout << hashInfo[(unsigned char)root->t.ch].code << std::endl; // test
			return;
		}

		if (root->left)
		{
			root->left->t.code += root->t.code + '0';
			std::cout << root->left->t.code << std::endl; // test thử
		}
		if (root->right)
		{
			root->right->t.code = root->t.code + '1';
			std::cout << root->right->t.code << std::endl; // test thử
		}

		if (root->left)
			MakeCode(root->left);
		if (root->right)
			MakeCode(root->right);
	}


	void UnCompress(const string& CompressFile)
	{
		ifstream ifs(CompressFile, ios::binary);
		size_t suf_count = 0;
		// đọc lại hậu tố của tệp cũ, hậu tố này đã được ghi ở đầu file mã hóa
		ifs.read((char*)&suf_count, sizeof(size_t));
		char bufsuf[50] = { '\0' };
		ifs.read(bufsuf, suf_count);
		string suf(bufsuf);

		cout << "Nhap ten tap can giai nen: " << endl;
		string newfile;
		cin >> newfile;

		newfile += suf;
		const string newfilename(newfile);

		ofstream ofs(newfilename, ios::binary);
		CharInfo chInfo;

		while (1)
		{
			ifs.read((char*)&chInfo, sizeof(chInfo));

			if (chInfo.count > 0)
			{
				hashInfo[(unsigned char)chInfo.ch].count = chInfo.count;

			}
			else
			{
				break;
			}
		}

		// xây lại cây huffman theo hashInfo
		HashInfo illegal;
		illegal.count = 0;

		HuffmanTree<HashInfo> tree(hashInfo, 256, illegal);

		// khôi phục dữ liệu dựa trên cây huffman được xây dựng
		int pos = 0;
		char c = '\0';
		Node* root = tree.getRoot();
		int count = root->t.count;
		Node* ptr = root;
		while (ifs.get(c))
		{
			for (size_t pos = 0; pos < 8; ++pos)
			{
				if (count == 0)
				{
					break;
				}

				if ((1 << pos & c) != 0)
				{
					if (ptr->right)
						ptr = ptr->right;
				}
				else if ((1 << pos & c) == 0)
				{
					if (ptr->left)
						ptr = ptr->left;
				}

				if (ptr->left == nullptr && ptr->right == nullptr)
				{
					ofs.put(ptr->t.ch);
					ptr = root;
					count--;
				}
			}
		}

	}
};


void TestCompress(const string &file)
{
	FileCompress f;
	f.Compress(file);
}

void TestUnCompress(const string &file)
{
	FileCompress f;
	f.UnCompress(file);
}