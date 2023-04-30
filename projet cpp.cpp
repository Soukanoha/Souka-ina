
	#include <iostream>
#include <string>
#include <mutex>
#include <condition_variable>

using namespace std;

class Stock {
public:
    Stock(int initial_quantity) {
        quantity = initial_quantity;
    }

    void add(int amount) {
        unique_lock<mutex> lock(m_mutex);
        m_cond_var.wait(lock, [this, amount]() {
            return (quantity + amount) >= 0;
        });
        quantity += amount;
    }

    bool remove(int amount) {
        unique_lock<mutex> lock(m_mutex);
        m_cond_var.wait(lock, [this, amount]() {
            return (quantity - amount) >= 0;
        });
        quantity -= amount;
        return true;
    }

    int get_quantity() const {
        unique_lock<mutex> lock(m_mutex);
        return quantity;
    }

private:
    int quantity;
    mutable mutex m_mutex;
    condition_variable m_cond_var;
};

int main() {
    Stock inventory(100); // stock initial de 100 articles
    int choice, amount;
    bool quit = false;

    while (!quit) {
        cout << "Que voulez-vous faire ?" << endl;
        cout << "1. Ajouter des articles" << endl;
        cout << "2. Enlever des articles" << endl;
        cout << "3. Afficher le stock actuel" << endl;
        cout << "4. Quitter" << endl;
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Combien d'articles voulez-vous ajouter ?" << endl;
                cin >> amount;
                inventory.add(amount);
                break;
            case 2:
                cout << "Combien d'articles voulez-vous enlever ?" << endl;
                cin >> amount;
                if (inventory.remove(amount)) {
                    cout << "Articles enlevés." << endl;
                } else {
                    cout << "Il n'y a pas assez de stock pour enlever " << amount << " articles." << endl;
                }
                break;
            case 3:
                cout << "Le stock actuel est de " << inventory.get_quantity() << " articles." << endl;
                break;
            case 4:
                quit = true;
                break;
            default:
                cout << "Choix invalide. Veuillez choisir une option valide." << endl;
                break;
        }
    }

    
	return 0;
}
