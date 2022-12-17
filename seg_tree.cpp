#include <iostream>
#include <cmath>
#include <vector>
using namespace std;
/******************************************************************/
/*                              _\/_                              */
/*                               /\                               */
/*                               /\                               */
/*      Epico Alberello         /  \                              */
/*                              /~~\o                             */
/*                             /o   \       Peccato che i grafi   */
/*                            /~~*~~~\        non siano belli     */
/*                           o/    o \          come questo       */
/*                           /~~~~~~~~\~`    mitico alberello     */
/*                          /__*_______\                          */
/*      Bisogna saperli          ||                               */
/*         apprezzare          \====/                             */
/*                              \__/                              */
/******************************************************************/
/*                                                                */
/*                                                                */
/*                         I segment tree                         */
/*                             by Ben                             */
/*                                                                */
/*                                                                */
/*                        #YellowRadiators                        */
/******************************************************************/

/**
 * Perche' usare i segment tree?
 * I segment tree permettono di computare la somma di un range (ma non solo!)
 * in tempo O(log n) invece che per O(n) come avverrebbe con un array.
 * 
 * L'unico "downside" e' il tempo di aggiornamento che e' O(log n) invece che O(1)
 * 
 * Vuoi avere una struttura che ti permette di sapere la somma di un range in tempo O(1)
 * ma con un tempo di aggiornamento O(n)?
 * Prova con un Prefix Sum Array!
 * 
 * Alternativamente al Segment Tree puoi usare un Fenwick Tree (Binary Indexed Tree)
 * 
 * Per maggiori informazioni: https://www.quora.com/How-does-one-decide-when-to-use-a-Segment-Tree-or-Fenwick-Tree
*/

template<typename T>
class SegmentTree {
    /**
     * Per creare questa classe ho usato le seguenti fonti:
     * https://cp-algorithms.com/data_structures/segment_tree.html
     * https://www.geeksforgeeks.org/segment-tree-sum-of-given-range/
     * 
     * (usato nel senso che le ho studiate, le ho capite a fondo e poi le ho implementate)
     * 
     * NOTA: Questo e' un segment tree per la somma di un range, ma puoi facilmente estenderlo
     *       per fare altre cose come la minima o massima di un range (leggi la prima fonte)
    */

    private:
        vector<T> tree;

        // Dimensione dell'array di input
        size_t arr_size;

        /**
         * @brief Costruisce il segment tree dato un array
         * 
         * @param arr std::vector da cui costruire il segment tree
         * @param left Indice di partenza dell'array
         * @param right Indice di fine dell'array
         * @param st_index Indice del segment tree
         * 
         * @return Il valore del nodo st_index (serve solo per la ricorsione)
        */
        T build_tree(const vector<T>& arr, 
                     const size_t& left, 
                     const size_t& right, 
                     const size_t& st_index) 
        {
            // Se i range coincidono allora siamo arrivati ad una "foglia"
            // Cos'e' una foglia? E' un nodo che non ha figli!
            if (left == right) {
                this->tree[st_index] = arr[left];
                return arr[left];
            }

            // Se c'e' piu' di un elemento allora dobbiamo dividere l'array in due
            size_t mid = left + (right - left) / 2;

            // La somma dei due sottoalberi e' il valore del nodo corrente
            this->tree[st_index] = this->build_tree(arr, left, mid, 2 * st_index + 1) + 
                                   this->build_tree(arr, mid + 1, right, 2 * st_index + 2);
            /**
             * Ok, tutto chiaro... ma perche' 2 * st_index + 1 e 2 * st_index + 2?
             * 2 * st_index + 1 rappresenta if figlio sinistro del nodo st_index-esimo
             * 2 * st_index + 2 rappresenta if figlio destro del nodo st_index-esimo
             * 
             * Per esempio, il nodo st_index = 0 contiene la somma di tutti gli elementi
             * dell'array, il nodo st_index = 1 contiene la somma di tutti gli elementi
             * dell'array che si trovano a sinistra del nodo st_index = 0 (e quindi e' suo figlio)
             * il nodo st_index = 2 contiene la somma di tutti gli elementi st_index = 0 
             * (anche lui e' suo figlio)
             * 
             * Se hai qualche dubbio a riguardo ti suggerisco di farti un esempio su 
             * paint o su carta e di vedere come viene rappresentato l'albero,
             * sia graficamente che come nell'array.
             * 
             * Pro-tip: fatti un esempio con max. 5 elementi :)
            */

            // Non scordiamoci di restituire il valore del nodo corrente
            return this->tree[st_index];
        }
        
        /**
         * @brief Calcola la somma di una query [x, y]
         * 
         * @param query_start Inizio del range della query
         * @param query_end Fine del range della query
         * @param left Inidio del range del nodo corrente
         * @param right Fine del range del nodo corrente
         * @param st_index Indice del nodo corrente
         * 
         * @return Somma del range della query
        */
        T get_sum_helper(const size_t& query_start,
                         const size_t& query_end,
                         const size_t& left,
                         const size_t& right,
                         const size_t& st_index)
        {
            // Se il range del nodo corrente e' contenuto nel range della query
            // allora possiamo restituire il valore del nodo corrente
            if (query_start <= left && query_end >= right) {
                return this->tree[st_index];
            }

            // Se il range del nodo corrente non e' contenuto nel range della query
            // allora dobbiamo restituire 0
            if ((right < query_start) || (left > query_end)) {
                return 0;
            }

            // Se il range del nodo corrente e' parzialmente contenuto nel range della query
            // allora dobbiamo dividere il nodo corrente in due e sommare i due sottoalberi
            size_t mid = left + (right - left) / 2;
            return this->get_sum_helper(query_start, query_end, left, mid, 2 * st_index + 1) +
                   this->get_sum_helper(query_start, query_end, mid + 1, right, 2 * st_index + 2);

            /**
             * Il passaggio finale e' probabilmente il piu' complicato da capire ma anche il piu' fondamentale.
             * Si capisce molto meglio facendo un esempio!
             * 
             * Ipotizziamo di avere la query [0, 2] ed al momento siamo nel nodo che copre gli indici [0, 7]...
             * 
             * Cosa succede?
             * Innanzitutto dobbiamo dividere il nodo corrente in due e sommare i due sottoalberi
             * NOTA: Sommare non significa per forza che ci sara' un numero diverso da 0 per entrambi i sottoalberi
             * 
             * Dalla divisione otteniamo i nodi che rappresentano [0, 3] e [4, 7]  
             * Possiamo scartare il secondo sottoalbero [4, 7] perche' uno dei due valori non e' nel range della query
             * 
             * Adesso abbiamo il nodo [0, 3], da cui seguira'...
             *    [0, 3]
             *    /     \
             * [0, 1] [2, 3]
             * 
             * [0, 1] e' un nodo che e' contenuto completamente nella query, per cui ritorniamo il suo valore
             * 
             * [2, 3] e' parzialmente nella query, per cui avremo...
             *   [2, 3]
             *   /    \
             * [2, 2] [3, 3]
             * 
             * [2, 2] e' un nodo che e' contenuto completamente nella query, per cui ritorniamo il suo valore
             * [3, 3] e' un nodo che e' contenuto completamente nella query, per cui ritorniamo il suo valore
             * 
             * In conclusione, sum = [0, 1] + [2, 2]
            */
        }

        /**
         * @brief Aggiorna il valore di un elemento nell'albero
         * 
         * @param difference Differenza tra il vecchio e il nuovo valore
         * @param index Indice dell'elemento da aggiornare
         * @param left Inizio del range del nodo corrente
         * @param right Fine del range del nodo corrente
         * @param st_index Indice del nodo corrente
         * 
         * @return void, aggiorna solamente i valori dell'albero
        */
        void update_helper(const T& difference,
                           const size_t& index,
                           const size_t& left,
                           const size_t& right,
                           const size_t& st_index)
        {
            // Se l'indice non e' nel range del nodo corrente (caso base)
            // allora non deve essere aggiornato
            if (index < left || index > right) {
                return;
            }

            // Se, invece, l'indice e' nel range del nodo corrente
            // allora dobbiamo aggiornare il valore del nodo corrente
            // (e dei suoi sottoalberi)
            this->tree[st_index] += difference;

            // Ovviamente, aggiornamento i sottoalberi solo se il nodo corrente non e' una foglia
            // Cos'e' una foglia? E' un nodo che non ha figli!
            if(left == right) return;

            size_t mid = left + (right - left) / 2;

            // Aggiorniamo l'albero sinistro
            this->update_helper(difference, index, left, mid, 2 * st_index + 1);

            // Aggiorniamo l'albero destro
            this->update_helper(difference, index, mid + 1, right, 2 * st_index + 2);

            /**
             * Qua non c'e' molto da spiegare, se l'indice e' nel range del nodo corrente
             * allora aggiorniamo il valore del nodo corrente e dei suoi sottoalberi (se ne ha)
            */
        }
    public:
        // Array di input
        // L'ho messo qua dentro in modo tale da non "sporcare" l'array originale
        // ma, in caso ti servisse puoi sempre accedervi dato che l'ho messo pubblico
        vector<T> arr;

        /**
         * @brief Costruttore della classe, costruisce automaticamente l'albero
         * 
         * @param arr Array con cui costruire l'albero
         * 
         * @return SegmentTree
        */
        SegmentTree(const vector<T>& arr) {
            // Se vuoi capirlo e' meglio che vai su GeeksForGeeks bruh
            // PS: Sarebbe 2 * 2^log2(n) - 1 (dove n e' la dimensione dell'array)
            this->arr = arr;
            this->arr_size = arr.size();

            size_t size = 2 * pow(2, ceil(log2(this->arr_size))) - 1;
            this->tree.resize(size);

            this->build_tree(arr, 0, this->arr_size - 1, 0);
        }

        /**
         * @brief Wrapper per calcolare la somma di un range passando solamente il range della query
         * 
         * @param query_start Inizio del range della query
         * @param query_end Fine del range della query
         * 
         * @return La somma di tutti gli elementi compresi tra query_start e query_end
        */
        T get_sum(const size_t& query_start,
                  const size_t& query_end)
        {
            // Ovviamente non puoi fare la somma di un range che non esiste
            // ma siccome i controlli sono per le pussy noi non li facciamo
            return this->get_sum_helper(query_start, query_end, 0, this->arr_size - 1, 0);
        }

        /**
         * @brief Wrapper per aggiornare l'elemento di indice index con un nuovo valore
         * 
         * @param index Indice dell'elemento da aggiornare
         * @param new_val Nuovo valore dell'elemento
        */
        void update(const size_t& index,
                    const T& new_val)
        {
            // Ovviamente non puoi fare l'update di un elemento che non esiste
            // ma siccome i controlli sono per le pussy noi non li facciamo
            T diff = new_val - this->arr[index];

            this->arr[index] = new_val;

            this->update_helper(diff, index, 0, this->arr_size - 1, 0);
        }
};

int main(int argc, char** argv) {
    vector<int> arr = {1, 3, 5, 7, 9, 11};

    SegmentTree<int> st(arr);

    cout << st.get_sum(0, 1) << endl; // 4

    st.update(1, 9); // { 1, 9, 5, 7, 9, 11 }

    cout << st.get_sum(0, 1) << endl; // 10

    // Cambia l'ultimo elemento in modo tale
    // che la somma totale dell'array risulti 100
    int sum = st.get_sum(0, arr.size() - 2);
    st.update(arr.size() - 1, 100 - sum);

    cout << st.get_sum(0, arr.size() - 1) << endl; // 100
}