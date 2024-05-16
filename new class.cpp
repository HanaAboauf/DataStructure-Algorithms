// last version

#include <iostream>
#include <functional>
#include <limits>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class item {
private:
    string itemname;
    string category;
    int price;
    static function<bool(const item&, const item&)> compare;
public:
    item() : itemname(" "), category(" "), price(0) {}
    item(string name, string categ, int pri) : itemname(name), category(categ), price(pri) {}

    static void CompareItems(function<bool(const item&, const item&)> comp) {
        compare = comp;
    }

    bool operator<(const item& other) const {
        return compare(*this, other);
    }

    bool operator!=(const item& other) const {
        return itemname != other.itemname || category != other.category || price != other.price;
    }

    bool operator>(const item& other) const {
        return !compare(*this, other) && !(*this == other);
    }

    bool operator==(const item& other) const {
        return itemname == other.itemname && category == other.category && price == other.price;
    }

    friend ostream& operator<<(ostream &out, const item &itm) {
        out << "Name:" << itm.itemname << " Category:" << itm.category << " Price:" << itm.price << endl;
        return out;
    }

    string getItemname() const {
        return itemname;
    }

    string getCategory() const {
        return category;
    }

    int getPrice() const {
        return price;
    }

    void setItemname(const string &itemname) {
        this->itemname = itemname;
    }

    void setCategory(const string &category) {
        this->category = category;
    }

    void setPrice(int price) {
        this->price = price;
    }
};

function<bool(const item&, const item&)> item::compare = [](const item& a, const item& b) {
    return a.getItemname() < b.getItemname();
};

template<class t>
class node {
private:
    t key;
    node<t>* left;
    node<t>* right;

public:
    node() : left(nullptr), right(nullptr) {}
    node(const t& el, node* r = nullptr, node* l = nullptr) : key(el), left(l), right(r) {}

    t getKey() {
        return key;
    }

    node<t>* getLeft() {
        return left;
    }

    void setLeft(node<t>* l) {
        left = l;
    }

    node<t>* getRight() {
        return right;
    }

    void setRight(node<t>* r) {
        right = r;
    }

    void setKey(const t& k) {
        key = k;
    }
};

template<class t>
class BST {
protected:
    node<t>* root;

public:
    BST() : root(nullptr) {}

    void addItem(t el) {
        node<t>* p = root;
        node<t>* pre = nullptr;

        while (p != nullptr) {
            pre = p;
            if (p->getKey() > el)
                p = p->getLeft();
            else if (p->getKey() < el)
                p = p->getRight();
        }

        if (root == nullptr)
            root = new node<t>(el);
        else if (pre->getKey() > el)
            pre->setLeft(new node<t>(el));
        else
            pre->setRight(new node<t>(el));
    }

    node<t>* predecessor(node<t>* p) {
        if (p == nullptr || p->getLeft() == nullptr)
            return nullptr;

        p = p->getLeft();
        while (p->getRight() != nullptr)
            p = p->getRight();

        return p;
    }

    bool noChild(node<t>* p) const {
        return (p->getLeft() == nullptr && p->getRight() == nullptr);
    }

    bool hasOneRChild(node<t>* p) const {
        return (p->getLeft() == nullptr && p->getRight() != nullptr);
    }

    bool hasOneLChild(node<t>* p) const {
        return (p->getLeft() != nullptr && p->getRight() == nullptr);
    }

    bool has2Children(node<t>* p) const {
        return (p->getLeft() != nullptr && p->getRight() != nullptr);
    }

    void removeItem(t el) {
        node<t>* p = root;
        node<t>* parent = nullptr;

        while (p != nullptr && p->getKey() != el) {
            parent = p;
            if (el < p->getKey())
                p = p->getLeft();
            else
                p = p->getRight();
        }

        if (p == nullptr)
            return;

        if (noChild(p)) {
            if (parent == nullptr)
                root = nullptr;
            else if (parent->getLeft() == p)
                parent->setLeft(nullptr);
            else
                parent->setRight(nullptr);
            delete p;
        } else if (hasOneRChild(p)) {
            if (parent == nullptr)
                root = p->getRight();
            else if (parent->getLeft() == p)
                parent->setLeft(p->getRight());
            else
                parent->setRight(p->getRight());
            delete p;
        } else if (hasOneLChild(p)) {
            if (parent == nullptr)
                root = p->getLeft();
            else if (parent->getLeft() == p)
                parent->setLeft(p->getLeft());
            else
                parent->setRight(p->getLeft());
            delete p;
        } else if (has2Children(p)) {
            node<t>* predecessorNode = predecessor(p);
            t predecessorKey = predecessorNode->getKey();
            removeItem(predecessorKey);
            p->setKey(predecessorKey);
        }
    }

    void inOrderTraversal(node<t>* p, function<void(const t&)> visit) const {
        if (p != nullptr) {
            inOrderTraversal(p->getLeft(), visit);
            visit(p->getKey());
            inOrderTraversal(p->getRight(), visit);
        }
    }

    void inOrder(function<void(const t&)> visit) const {
        inOrderTraversal(root, visit);
    }

    void inOrderReverseTraversal(node<t>* p, function<void(const t&)> visit) const {
        if (p != nullptr) {
            inOrderReverseTraversal(p->getRight(), visit);
            visit(p->getKey());
            inOrderReverseTraversal(p->getLeft(), visit);
        }
    }

    void inOrderReverse(function<void(const t&)> visit) const {
        inOrderReverseTraversal(root, visit);
    }

};

void readItems(istream& inputFile, BST<item>& tree) {
    int numItems;
    inputFile >> numItems;
    inputFile.ignore(numeric_limits<streamsize>::max(), '\n');
    for (int i = 0; i < numItems; ++i) {
        string name, category;
        int price;
        getline(inputFile, name);
        getline(inputFile, category);
        inputFile >> price;
        inputFile.ignore(numeric_limits<streamsize>::max(), '\n');
        tree.addItem(item(name, category, price));
    }
}


int main() {
    BST<item> bstN, bstP;
    vector<item>qu;

    item::CompareItems([](const item& a, const item& b) {
        return a.getItemname() < b.getItemname();
    });

    int choice;

    do {
        cout << "\nBinary Search Trees (BST) Menu\n";
        cout << "1. Add item\n";
        cout << "2. Remove item\n";
        cout << "3. Display item data normally\n";
        cout << "4. Display items sorted by name ascending\n";
        cout << "5. Display items sorted by name descending\n";
        cout << "6. Display items sorted by price ascending\n";
        cout << "7. Display items sorted by price descending\n";
        cout << "8. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                cout << "for file enter 'f' or manually enter 'm'\n";
                char c;
                cin >> c;
                if (c == 'm') {
                    string name, category;
                    int price;
                    cout << "Enter item name: ";
                    cin >> name;
                    cout << "Enter item category: ";
                    cin >> category;
                    cout << "Enter item price: ";
                    cin >> price;
                    item newItem(name, category, price);
                    qu.push_back(newItem);

                    item::CompareItems([](const item& a, const item& b) {
                        return a.getItemname() < b.getItemname();
                    });
                    bstN.addItem(newItem);


                    item::CompareItems([](const item& a, const item& b) {
                        return a.getPrice() < b.getPrice();
                    });
                    bstP.addItem(newItem);
                } else if (c == 'f') {
                    ifstream infile("usereading.txt");

                    item::CompareItems([](const item& a, const item& b) {
                        return a.getItemname() < b.getItemname();
                    });
                    readItems(infile, bstN);

                    infile.clear();
                    infile.seekg(0, ios::beg);

                    item::CompareItems([](const item& a, const item& b) {
                        return a.getPrice() < b.getPrice();
                    });
                    readItems(infile, bstP);

                    infile.close();
                }

                ifstream file("usereading.txt");
                if (!file.is_open()) {
                    cout << "Can't open file!" << endl;
                    return 1;
                }

                int numStudents;
                file >> numStudents;
                file.ignore();

                for (int i = 0; i < numStudents; ++i) {
                    string name, category;
                    int price;

                    getline(file, name);
                    getline(file, category);
                    file >> price;

                    file.ignore(numeric_limits<streamsize>::max(), '\n');

                    item newItem(name, category, price);
                    qu.push_back(newItem);
                }

                file.close();
                break;
            }
            case 2: {
                string name, category;
                int price;
                cout << "Enter item name to remove: ";
                cin >> name;
                cout << "Enter item category: ";
                cin >> category;
                cout << "Enter item price: ";
                cin >> price;
                item itemToRemove(name, category, price);

                item::CompareItems([](const item& a, const item& b) {
                    return a.getItemname() < b.getItemname();
                });
                bstN.removeItem(itemToRemove);

                item::CompareItems([](const item& a, const item& b) {
                    return a.getPrice() < b.getPrice();
                });
                bstP.removeItem(itemToRemove);
                break;
            }
            case 3: {
                cout << "Displaying items normally:\n";

                for(item i:qu)
                    cout<<"item name: "<<i.getItemname()<<" "<<"category: "<<i.getCategory()<<" "<<"price: $"<<i.getPrice()<<"\n";

                break;
            }
            case 4: {
                cout << "Displaying items sorted by name ascending:\n";
                bstN.inOrder([](const item& it) {
                    cout << it;
                });
                break;
            }
            case 5: {
                cout << "Displaying items sorted by name descending:\n";
                bstN.inOrderReverse([](const item& it) {
                    cout << it;
                });
                break;
            }
            case 6: {
                cout << "Displaying items sorted by price ascending:\n";
                bstP.inOrder([](const item& it) {
                    cout << it;
                });
                break;
            }
            case 7: {
                cout << "Displaying items sorted by price descending:\n";
                bstP.inOrderReverse([](const item& it) {
                    cout << it;
                });
                break;
            }
            default:
                if (choice != 8)
                    cout << "Invalid choice!\n";
                break;
        }
    } while (choice != 8);

    return 0;
}
