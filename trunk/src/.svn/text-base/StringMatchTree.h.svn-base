#ifndef _STRINGMATCHTREE_H_
#define _STRINGMATCHTREE_H_

#include <iostream>
#include <string>
#include <vector>
#include <utility>

using namespace std;

template <class T>
class StringMatchTree
{
	public:

	class iterator
	{
		public:
		iterator();
		iterator(const iterator& i);
		iterator(StringMatchTree<T> *i, bool n);

		iterator& operator--();
		iterator& operator++();

		pair<const string, T&> operator*();

		bool operator==(const iterator& o);
		bool operator!=(const iterator& o);

		public:
		vector<StringMatchTree<T> *> treestack;
		vector<int> countstack;
		bool isend;

		void next_subnode();
		void backtrack();

		void traverse(bool flag);
	};

	StringMatchTree();
	~StringMatchTree();

	T& Match(string s);

	T& Get(string s);
	T& Set(string s, T& newitem);
	void Clear(string s);

	void Dump(string& s, int depth = 0);

	static int differentat(string s1, string s2);

	iterator begin();
	iterator end();

	private:
	StringMatchTree(string ns, T& ni);
	StringMatchTree(string ns);

	void Assign(T& newitem);

	static StringMatchTree<T> * vecsearch(string searchfor, vector<StringMatchTree<T> *>& searchin);
	static StringMatchTree<T> *& vecsearchpartial(string searchfor, vector<StringMatchTree<T> *>& searchin);
	static StringMatchTree<T> *& vecsearchexact(string searchfor, vector<StringMatchTree<T> *>& searchin);

	static bool vecinsert(vector<StringMatchTree<T> *>& toupdate, StringMatchTree<T> * n);
	static bool vecerase(vector<StringMatchTree<T> *>& toupdate, string s);

	string s;
	T * item;
	vector<StringMatchTree<T> *> t;
};

template <typename T> typename StringMatchTree<T>::iterator StringMatchTree<T>::begin()
{
	return StringMatchTree<T>::iterator(this, false);
}

template <typename T> typename StringMatchTree<T>::iterator StringMatchTree<T>::end()
{
	return StringMatchTree<T>::iterator(this, true);
}

template <typename T> StringMatchTree<T>::iterator::iterator()
{
	isend = true;
}

template <typename T> StringMatchTree<T>::iterator::iterator(StringMatchTree<T> *p, bool n)
{
	isend = n;
	treestack.push_back(p);
	countstack.push_back(0);
	traverse(true);
}

template <typename T> StringMatchTree<T>::iterator::iterator(const StringMatchTree<T>::iterator& i)
{
	isend = i.isend;
	treestack = i.treestack;
	countstack = i.countstack;
}

template <typename T> typename StringMatchTree<T>::iterator& StringMatchTree<T>::iterator::operator--()
{
	// not yet implemented
}

template <typename T> typename StringMatchTree<T>::iterator& StringMatchTree<T>::iterator::operator++()
{
	traverse(false);
}

template <typename T> pair<const string, T&> StringMatchTree<T>::iterator::operator*()
{
	string s;
	for (int i = 0; i < treestack.size(); ++i) s += treestack[i]->s;
	return pair<const string, T&>(s, *(treestack.back()->item));
}

template <typename T> bool StringMatchTree<T>::iterator::operator==(const StringMatchTree<T>::iterator& o)
{
	if (isend && o.isend && o.treestack.size() != 0 && treestack.size() != 0 && o.treestack.at(0) == treestack.at(0)) return true;
	return (treestack == o.treestack) && (countstack == o.countstack);
}

template <typename T> bool StringMatchTree<T>::iterator::operator!=(const StringMatchTree<T>::iterator& o)
{
	return !(*this == o);
}

template <typename T> void StringMatchTree<T>::iterator::next_subnode()
{
	treestack.push_back(treestack.back()->t.at(countstack.back()));
	countstack.push_back(0);
}

template <typename T> void StringMatchTree<T>::iterator::backtrack()
{
	if (treestack.size() == 1)
	{
		isend = true;
		return;
	}
	treestack.pop_back();
	countstack.pop_back();
	++(countstack.back());
}

template <typename T> void StringMatchTree<T>::iterator::traverse(bool flag)
{
	if (flag && (treestack.back()->item != NULL))
		return;
	while (!isend)
	{
		while (treestack.back()->t.size() <= countstack.back())
		{
			if (isend) return;
			backtrack();
		}
		next_subnode();
		if (treestack.back()->item != NULL)
			return;
	}
}

template <typename T> void do_dump(StringMatchTree<T>& t)
{
	string s = "";
	t.Dump(s);
	cout << s;
}

template <class T> StringMatchTree<T>::StringMatchTree(string ns, T& ni)
{
	s = ns;
	item = new T(ni);
}

template <class T> StringMatchTree<T>::~StringMatchTree()
{
	if (item != NULL) delete item;
	for (typename vector<StringMatchTree<T> *>::iterator i = t.begin(); i != t.end(); ++i)
		delete (*i);
}

template <class T> StringMatchTree<T>::StringMatchTree(string ns)
{
	s = ns;
	item = NULL;
}

template <class T> StringMatchTree<T>::StringMatchTree()
{
	s = "";
	item = NULL;
}

template <class T> void StringMatchTree<T>::Assign(T& newitem)
{
	if (item != NULL)
		delete item;
	item = new T(newitem);
}

template <class T> void StringMatchTree<T>::Dump(string& data, int depth)
{
	data.append(depth, ' '); data.append(string("Key: ") + s + "\n");
	//data.append(depth, ' '); data.append(string("Value: ") + (item != NULL ? *item : "[NULL]") + "\n");
	if (t.size() == 0) return;
	data.append(depth, ' '); data.append("[\n");
	for (typename vector<StringMatchTree<T> *>::iterator i = t.begin(); i != t.end(); ++i)
		(*i)->Dump(data, depth + 1);
	data.append(depth, ' '); data.append("]\n");
}

template <class T> void StringMatchTree<T>::Clear(string s)
{
	string olds = s;
	StringMatchTree<T> * current = this, * next, * previous = NULL;
	if (current == NULL)
		return;
	while (true)
	{
		StringMatchTree<T> *& temp = vecsearchexact(s, current->t);
		if (&temp == NULL)
			break;
		next = temp;
		previous = current;
		current = next;
		s = s.substr(next->s.length());
	}
	if (s != "")
		return;
	if (current->t.size() == 0 && previous != NULL)
	{
		do_dump<T>(*previous);
		vecerase(previous->t, current->s);
		do_dump<T>(*previous);
		string p = olds.substr(0, olds.length() - current->s.length());
		delete current;
		if (p != "")
			Clear(p);
	}
	else if (current->t.size() == 1 && previous != NULL)
	{
		next = *(current->t.begin());
		next->s = current->s + next->s;
		vecsearchexact(current->s, previous->t) = next;
		current->t.clear();
		delete current;
	}
	else if (current->item != NULL)
	{
		delete current->item;
		current->item = NULL;
	}
}

template <class T> T& StringMatchTree<T>::Match(string s)
{
	StringMatchTree<T> * current = this, * next;
	if (current == NULL)
		return (*(T *)NULL);
	while (true)
	{
		next = vecsearch(s, current->t);
		if (next == NULL)
			return *(current->item);
		s = s.substr(next->s.length());
		current = next;
	}
}

template <class T> T& StringMatchTree<T>::Get(string s)
{
	StringMatchTree<T> * current = this;
	if (current == NULL)
		return (*(T *)NULL);
	while (true)
	{
		StringMatchTree<T> *& next = vecsearchexact(s, current->t);
		if (&next == NULL)
			return *(current->item);
		s = s.substr(next->s.length());
		current = next;
	}
}

template <class T> T& StringMatchTree<T>::Set(string s, T& newitem)
{
	if (t.size() == 0)
	{
		t.push_back(new StringMatchTree<T>(s, newitem));
		return *(t[0]->item);
	}
	if (s.length() == 0)
	{
		Assign(newitem);
		return *(this->item);
	}
	string c = s.substr(0, 1);
	StringMatchTree<T> *& next = vecsearchpartial(c, t);
	if (&next == NULL || next->s.at(0) != c.at(0))
	{
		StringMatchTree<T> * n = new StringMatchTree<T>(s, newitem);
		vecinsert(t, n);
		return *(n->item);
	}
	else
	{
		int d = differentat(s, next->s); d = (d < 0 ? -d : d);
		if (d == s.length() && d == next->s.length())
			return next->Set("", newitem);
		else
		{
			string common = s.substr(0, d);
			string news = s.substr(d);
			string olds = next->s.substr(d);
			if (d == next->s.length())
				return next->Set(news, newitem);
			else
			{
				StringMatchTree<T> * n = new StringMatchTree<T>(common);
				StringMatchTree<T> * o = next;
				o->s = olds;
				next = n;
				vecinsert(next->t, o);
				return next->Set(news, newitem);
			}
		}
	}
}

template <class T> StringMatchTree<T> * StringMatchTree<T>::vecsearch(string searchfor, vector<StringMatchTree<T> *>& searchin)
{
	int start = 0, end = searchin.size(), mid, areequal;
	if (end == 0)
	{
		return NULL;
	}
	while (true)
	{
		mid = (start + end) >> 1;
		areequal = searchin.at(mid)->s.compare(searchfor);
		if (areequal == 0)
			return searchin.at(mid);
		else if (searchfor.find(searchin.at(mid)->s) == 0)
		{
			StringMatchTree<T> * searchresult = vecsearch(searchfor.substr(searchin.at(mid)->s.length()), searchin.at(mid)->t);
			if (searchresult == NULL)
				return searchin.at(mid);
			return searchresult;
		}
		else if (start + 1 == end)
		{
			return NULL;
		}
		else if (areequal < 0)
			start = mid;
		else // if (areequal > 0)
			end = mid;
	}
}

template <class T> StringMatchTree<T> *& StringMatchTree<T>::vecsearchpartial(string searchfor, vector<StringMatchTree<T> *>& searchin)
{
	int start = 0, end = searchin.size(), mid, areequal;
	if (end == 0)
	{
		return *((StringMatchTree<T> **)NULL);
	}
	while (true)
	{
		mid = (start + end) >> 1;
		areequal = searchin.at(mid)->s.compare(searchfor);
		if (areequal == 0)
			return searchin.at(mid);
		else if (searchin.at(mid)->s.find(searchfor) == 0)
		{
			return searchin.at(mid);
		}
		else if (start + 1 == end)
		{
			return *((StringMatchTree<T> **)NULL);
		}
		else if (areequal < 0)
			start = mid;
		else // if (areequal > 0)
			end = mid;
	}
}

template <class T> StringMatchTree<T> *& StringMatchTree<T>::vecsearchexact(string searchfor, vector<StringMatchTree<T> *>& searchin)
{
	int start = 0, end = searchin.size(), mid, areequal;
	if (end == 0)
	{
		return *((StringMatchTree<T> **)NULL);
	}
	while (true)
	{
		mid = (start + end) >> 1;
		areequal = searchin.at(mid)->s.compare(searchfor);
		if (areequal == 0)
			return searchin.at(mid);
		else if (searchfor.find(searchin.at(mid)->s) == 0)
		{
			return searchin.at(mid);
			/*StringMatchTree<T> *& searchresult = vecsearchexact(searchfor.substr(searchin.at(mid)->s.length()), searchin.at(mid)->t);
			if (searchresult == NULL)
				return *((StringMatchTree<T> **)NULL);
			return searchresult; */
		}
		else if (start + 1 == end)
		{
			return *((StringMatchTree<T> **)NULL);
		}
		else if (areequal < 0)
			start = mid;
		else // if (areequal > 0)
			end = mid;
	}
}

template <class T> bool StringMatchTree<T>::vecinsert(vector<StringMatchTree<T> *>& toupdate, StringMatchTree<T> * n)
{
	int start = 0, end = toupdate.size(), mid, areequal;
	if (end == 0)
	{
		toupdate.push_back(n);
		return true;
	}
	while (true)
	{
		mid = (start + end) >> 1;
		areequal = toupdate.at(mid)->s.compare(n->s);
		if (start + 1 == end)
		{
			if (areequal < 0) toupdate.insert(toupdate.begin() + end, n);
			else if (areequal > 0) toupdate.insert(toupdate.begin() + start, n);
			else return false;
			return true;
		}
		if (areequal == 0)
			return false;
		else if (areequal < 0)
			start = mid;
		else // if (areequal > 0)
			end = mid;
	}
}

template <class T> bool StringMatchTree<T>::vecerase(vector<StringMatchTree<T> *>& toupdate, string s)
{
	int start = 0, end = toupdate.size(), mid, areequal;
	if (end == 0)
	{
		return false;
	}
	while (true)
	{
		mid = (start + end) >> 1;
		areequal = toupdate.at(mid)->s.compare(s);
		if (areequal == 0)
		{
			toupdate.erase(toupdate.begin() + mid);
			return true;
		}
		else if (start + 1 == end)
		{
			return false;
		}
		else if (areequal < 0)
			start = mid;
		else // if (areequal > 0)
			end = mid;
	}
}

template <class T> int StringMatchTree<T>::differentat(string s1, string s2)
{
	int l1 = s1.length(), l2 = s2.length();
	bool w = l1 > l2;
	bool e = l1 == l2;
	int m = (w ? l2 : l1);
	for (int i = 0; i < m; ++i)
	{
		if (s1.at(i) > s2.at(i)) return i;
		else if (s1.at(i) < s2.at(i)) return 0 - i;
	}
	if (e)
		return 0;
	if (w)
		return m;
	return 0 - m;
}

#endif
