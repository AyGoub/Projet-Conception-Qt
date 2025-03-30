
#pragma once
#include <vector>
#include <algorithm>

/**
 * @class IObserver
 * @brief Interface pour les classes qui souhaitent observer des objets Observable.
 * 
 * Cette interface définit le contrat que doivent implémenter tous les observateurs
 * dans le pattern Observer. Elle permet d'être notifié des changements qui surviennent
 * dans un objet Observable.
 */
class IObserver {
public:
    /**
     * @brief Méthode appelée lorsque l'objet observé change d'état
     * 
     * Cette méthode doit être implémentée par les classes dérivées pour
     * définir leur comportement lors de la notification d'un changement.
     */
    virtual void update() = 0;
    
    /**
     * @brief Destructeur virtuel par défaut
     */
    virtual ~IObserver() = default;
};

/**
 * @class Observable
 * @brief Classe de base pour les objets pouvant être observés.
 * 
 * Cette classe implémente le pattern Observer, permettant à d'autres objets
 * de s'enregistrer pour être notifiés des changements d'état. Elle fournit
 * les méthodes pour gérer les observateurs et déclencher les notifications.
 */
class Observable {
private:
    std::vector<IObserver*> observers;  /// Liste des observateurs enregistrés

public:
    /**
     * @brief Destructeur virtuel par défaut
     */
    virtual ~Observable() = default;

    /**
     * @brief Ajoute un observateur à la liste
     * @param observer Pointeur vers l'objet observateur à ajouter
     */
    void addObserver(IObserver* observer) {
        observers.push_back(observer);
    }

    /**
     * @brief Supprime un observateur de la liste
     * @param observer Pointeur vers l'objet observateur à supprimer
     */
    void removeObserver(IObserver* observer) {
        observers.erase(
            std::remove(observers.begin(), observers.end(), observer),
            observers.end()
            );
    }

    /**
     * @brief Notifie tous les observateurs enregistrés d'un changement d'état
     * 
     * Cette méthode parcourt la liste des observateurs et appelle leur méthode update()
     * pour les informer qu'un changement s'est produit dans l'objet Observable.
     */
    void notifyObservers() const {
        for (auto observer : observers) {
            observer->update();
        }
    }
};
