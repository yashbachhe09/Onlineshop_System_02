#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>

using namespace std;

// Structure to represent a product
struct Product {
    string name;
    double price;
    int frequency;
};

// Structure to represent a node in the binary search tree
struct Node {
    Product product;
    Node* left;
    Node* right;
};

// Function to create a new node
Node* createNode(Product product) {
    Node* newNode = new Node;
    newNode->product = product;
    newNode->left = newNode->right = nullptr;
    return newNode;
}

// Function to construct an optimal binary search tree
Node* constructOptimalBST(const vector<Product>& products, const vector<double>& frequencies, int start, int end) {
    if (start > end)
        return nullptr;

    double minCost = numeric_limits<double>::max();
    int minIndex = -1;

    // Find the root with the minimum cost
    double currentCost = 0;
    for (int i = start; i <= end; i++) {
        currentCost += frequencies[i];
        if (currentCost < minCost) {
            minCost = currentCost;
            minIndex = i;
        }
    }

    // Create the root node
    Node* root = createNode(products[minIndex]);

    // Construct the left and right subtrees recursively
    root->left = constructOptimalBST(products, frequencies, start, minIndex - 1);
    root->right = constructOptimalBST(products, frequencies, minIndex + 1, end);

    return root;
}

// Function to calculate the optimal layout of the tree and return the root node
Node* calculateOptimalLayout(const vector<Product>& products) {
    int n = products.size();
    vector<double> frequencies(n);

    // Calculate the frequencies of the products
    for (int i = 0; i < n; i++) {
        frequencies[i] = products[i].frequency;
    }

    // Construct the optimal binary search tree
    return constructOptimalBST(products, frequencies, 0, n - 1);
}

// Function to insert a product into the binary search tree
Node* insertProduct(Node* root, Product product) {
    if (root == nullptr) {
        return createNode(product);
    }

    // Recursively insert the product into the appropriate subtree
    if (product.name < root->product.name) {
        root->left = insertProduct(root->left, product);
    } else if (product.name > root->product.name) {
        root->right = insertProduct(root->right, product);
    } else {
        // Update the product information if it already exists in the tree
        root->product.price = product.price;
        root->product.frequency += product.frequency;
    }

    return root;
}

// Function to search for a product in the binary search tree
Node* searchProduct(Node* root, string name) {
    if (root == nullptr || root->product.name == name) {
        return root;
    }

    // Recursively search in the left or right subtree based on the product name
    if (name < root->product.name) {
        return searchProduct(root->left, name);
    } else {
        return searchProduct(root->right, name);
    }
}

// Function to delete a product from the binary search tree
Node* deleteProduct(Node* root, string name) {
    if (root == nullptr) {
        return root;
    }
            if (name < root->product.name) {
            root->left = deleteProduct(root->left, name);
        } else if (name > root->product.name) {
            root->right = deleteProduct(root->right, name);
        } else {
            // Node with only one child or no child
            if (root->left == nullptr) {
                Node* temp = root->right;
                delete root;
                return temp;
            } else if (root->right == nullptr) {
                Node* temp = root->left;
                delete root;
                return temp;
            }

            // Node with two children
            Node* minNode = root->right;
            while (minNode->left != nullptr) {
                minNode = minNode->left;
            }

            root->product = minNode->product;
            root->right = deleteProduct(root->right, minNode->product.name);
        }

        return root;
    }

    // Function to update the price of a product in the binary search tree
    void updatePrice(Node* root, string name, double newPrice) {
        Node* productNode = searchProduct(root, name);
        if (productNode != nullptr) {
            productNode->product.price = newPrice;
            cout << "Price updated successfully." << endl;
        } else {
            cout << "Product not found." << endl;
        }
    }

    // Function to traverse the products in the tree (inorder traversal)
    void traverseProducts(Node* root) {
        if (root == nullptr) {
            return;
        }

        traverseProducts(root->left);
        cout << "Product Name: " << root->product.name << ", Price: " << root->product.price << ", Frequency: " << root->product.frequency << endl;
        traverseProducts(root->right);
    }
// Function to generate a bill for the purchased products
double generateBill(Node* root, const vector<pair<string, int>>& purchasedProducts) {
    double totalBill = 0;

    for (const auto& purchase : purchasedProducts) {
        Node* productNode = searchProduct(root, purchase.first);
        if (productNode != nullptr) {
            double productPrice = productNode->product.price;
            int freq=productNode->product.frequency;
            int productQuantity = purchase.second;
            if(productQuantity>freq)
            {
                cout<<"Quantity exceeded than stock \n";
                break;
            }
           else{ double productTotal = productPrice * productQuantity;
            totalBill += productTotal;

            cout << "Product Name: " << productNode->product.name << ", Quantity: " << productQuantity << ", Total: " << productTotal << endl;
           }
        }
    }

    return totalBill;
}

    int main() {
        vector<Product> products;
        int n;

        Node* root = calculateOptimalLayout(products);

        while (true) {
            int choice;
            cout << "\nChoose an operation:\n";
            cout << "1. Insert a product\n";
            cout << "2. Search for a product\n";
            cout << "3. Delete a product\n";
            cout << "4. Update the price of product\n";
                        cout << "5. Traverse the products\n";
            cout << "6. Generate a bill\n";
            cout << "7. Exit\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1: {
                    // Code for inserting a product
                    Product product;
                    cout << "Enter the name of the product: ";
                    cin >> product.name;
                    cout << "Enter the price of the product: ";
                    cin >> product.price;
                    cout << "Enter the frequency of the product: ";
                    cin >> product.frequency;
                    root = insertProduct(root, product);
                    cout << "Product inserted successfully." << endl;
                    break;
                }
                case 2: {
                    // Code for searching a product
                    string name;
                    cout << "Enter the name of the product to search: ";
                    cin >> name;
                    Node* productNode = searchProduct(root, name);
                    if (productNode != nullptr) {
                        cout << "Product found: " << productNode->product.name << ", Price: " << productNode->product.price << ", Frequency: " << productNode->product.frequency << endl;
                    } else {
                        cout << "Product not found." << endl;
                    }
                    break;
                }
                case 3: {
                    // Code for deleting a product
                    string name;
                    cout << "Enter the name of the product to delete: ";
                    cin >> name;
                    root = deleteProduct(root, name);
                    cout << "Product deleted successfully." << endl;
                    break;
                }
                case 4: {
                    // Code for updating the price of a product
                    string name;
                    double newPrice;
                    cout << "Enter the name of the product to update: ";
                    cin >> name;
                    cout << "Enter the new price of the product: ";
                    cin >> newPrice;
                    updatePrice(root, name, newPrice);
                    break;
                }
                case 5: {
                    // Code for traversing the products
                    cout << "Products:\n";
                    traverseProducts(root);
                    break;
                }
                case 6: {
                // Code for generating a bill
                int numPurchased;
                cout << "Enter the number of products purchased: ";
                cin >> numPurchased;

                vector<pair<string, int>> purchasedProducts;
                for (int i = 0; i < numPurchased; i++) {
                    string name;
                    int quantity;
                    cout << "Enter the name of product " << i + 1 << ": ";
                    cin >> name;
                    cout << "Enter the quantity of product " << i + 1 << ": ";
                    cin >> quantity;
                    purchasedProducts.emplace_back(name, quantity);
                }

                cout << "\n.....................Bill.....................\n";
                double totalBill = generateBill(root, purchasedProducts);
                cout << "Total Bill: " << totalBill << endl;
                break;
            }
                case 7: {
                    cout << "Exiting the program.\n";
                    exit(0);
                }
                default:
                    cout << "Invalid choice. Please try again." << endl;
            }
        }

        return 0;
    }