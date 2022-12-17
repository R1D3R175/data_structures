#include <iostream> // std::cout, std::endl
#include <vector>   // std::vector
#include <stack>    // std::stack
#include <queue>    // std::queue
#include <limits>   // std::numeric_limits
#include <utility>  // std::pair
using namespace std;

/****************************************************************/
/*                    ---------|4|                              */
/*                    |        +-+                              */  
/*                 [9]|         |                               */
/*                    |      [3]|                               */
/*                   +-+       +-+  [11]  +-+                   */   
/*                   |5|-------|2|--------|3|                   */   
/*                   +-+  [1]  +-+        +-+                   */   
/*                    |         |          |                    */   
/*                [14]|     [10]|          |                    */   
/*                    |         |          |                    */   
/*                   +-+       +-+   [20]  |                    */   
/*                   |0|-------|1|----------                    */   
/*                   +-+  [7]  +-+                              */
/*                                                              */
/*                 Figura: Grafo bidirezionale                  */
/*                         pesato                               */
/****************************************************************/
/*                                                              */
/*                           I grafi                            */
/*                           by Ben                             */
/*                                                              */
/*                                                              */
/*                      #YellowRadiators                        */
/****************************************************************/

/**
 * Cos'e' un grafo?
 * Un grafo e' una struttura dati formata da un insieme di nodi (o vertici),
 * questi nodi sono collegati da archi, che possono essere pesati o non pesati.
 * Gli archi possono anche essere diretti o indiretti.
 * Diretto = A -> B, A puo' raggiungere B, ma B non puo' raggiungere A
 * Indiretto = A <-> B, A puo' raggiungere B e B puo' raggiungere A
 * 
 * Esistono vari metodi per rappresentare un grafo, in questo caso
 * utilizzeremo una lista di adiacenza.
 * 
 * La lista di adiacenza e' una struttura dati che rappresenta un grafo
 * tramite un array di liste, dove ogni lista rappresenta i nodi adiacenti
 * ad un nodo. (adiacenti = raggiungibili)
 * Esempio:
 * Ipotizziamo di avere un grafo bidirezionale con 4 nodi, e che il 
 * nodo 0 abbia degli archi con i nodi 1, 2 e 3. 
 * La nostra lista di adiacenza sara' cosi':
 * [0] -> 1, 2, 3
 * [1] -> 0
 * [2] -> 0
 * [3] -> 0
 * 
 * Nel caso di un grafo pesato ci sono due soluzioni:
 * 1. Creare una lista di adiacenza per i nodi e una lista di adiacenza per i pesi
 * 2. Creare una lista di adiacenza di coppie (nodo, peso)
 * 
 * Un altro metodo piu' comune per rappresentare un grafo, e'
 * la matrice di adiacenza, che e' una matrice quadrata di dimensione
 * V*V, dove V e' il numero di vertici del grafo.
 * 
 * Esempio:
 * Ipotizziamo di avere un grafo bidirezionale pesato con 4 nodi, e che il 
 * nodo 0 abbia degli archi con i nodi 1, 2 e 3. 
 * La matrice di adiacenza sara':
 * //  0   1   2   3
 * 0  INF  4   2   7
 * 1   4  INF INF INF
 * 2   2  INF INF INF
 * 3   7  INF INF INF
 * (si nota che la matrice e' simmetrica in quanto e' un grafo bidirezionale)
 * 
 * Si puo' interpretare nel seguente metodo:
 * matrice[0][1] == esiste un collegamento da 0 a 1? se si', quanto costa?
 * 
 * Ma se proprio volete fare i fighi esiste anche un metodo piu' ottimizzato per
 * ottimizzare le liste di adiacenza, e' il CSR (Compressed Sparse Row).
 * Si usa quando il grafo e' molto grande ma molto sparso.
 * Sparso = molti nodi con pochi archi
 * 
 * Se vuoi sapere in che modo sarebbe piu' ottimizzato, chiedi all'autore di questo file.
*/
template<typename T>
class Grafo {
    // Non possiamo avere indici negativi!
    static_assert(is_unsigned<T>::value, "T must be unsigned");

    private:
        vector<vector<T>> adjacency_list;
        vector<vector<T>> weights;

    public:
        /**
         * @brief Inizializza l'array di liste per il numero di nodi
         * 
         * @param n Numero di nodi
         * 
         * @return Grafo
        */
        Grafo(const size_t& n) {
            this->adjacency_list.resize(n);
            this->weights.resize(n);
        }

        /**
         * @brief Aggiunge un arco tra due nodi
         * 
         * @param from Nodo di partenza
         * @param to Nodo di arrivo
         * 
         * @return void
        */
        void add_edge(const T& from, const T& to, const T& weight) {
            this->adjacency_list[from].push_back(to);
            this->adjacency_list[to].push_back(from); // Se e' un grafo direzionale togli questa riga

            this->weights[from].push_back(weight);
            this->weights[to].push_back(weight); // Se e' un grafo direzionale togli questa riga

            /**
             * Piccola delucidazione su come ottenere il peso di un arco:
             * Dato che aggiungiamo un peso quando aggiungo l'arco, possiamo
             * semplicemente iterare nella lista di adiacenza di un nodo X
             * e contemporaneamente iterare nella lista dei pesi di X.
            */
        }

        /**
         * @brief Esegue una DFS (Depth First Search) dal nodo di partenza
         * 
         * @param src Nodo di partenza
         * @param dst Nodo di destinazione
         * 
         * @return Ritorna un vettore contenente il primo percorso trovato
        */
        vector<T> DFS(const T& src, const T& dst) {
            // path: contiene il nodo precedente di ogni nodo visitato
            vector<T> path(this->adjacency_list.size(), numeric_limits<T>::max());
            vector<bool> visited(this->adjacency_list.size(), false);

            // to_visit: struttura LIFO (Last In First Out)
            stack<T> to_visit;

            // Ovviamente partiamo dal nodo di partenza, no?
            to_visit.push(src);

            while(!to_visit.empty()) {
                // Prendiamo l'elemento piu' recentemente aggiunto
                T current = to_visit.top();
                to_visit.pop();

                // Se abbiamo raggiunto il nodo di destinazione, interrompiamo
                if(current == dst) break;

                // Se un nodo e' gia' stato visitato non lo visitiamo di nuovo
                if(visited[current]) continue;

                // Senno', lo marchiamo come visitato
                visited[current] = true;

                // Dopodiche', aggiungiamo tutti i nodi adiacenti a quelli visitati
                for(auto& n : this->adjacency_list[current]) {
                    if(!visited[n]) {
                        to_visit.push(n);

                        // Memorizziamo il nodo precedente per poter ricostruire il percorso
                        // n -> nodo che stiamo per visitare da current
                        path[n] = current;
                    }
                }                
            }
            
            vector<T> reconstructed_path;

            // Andiamo al contrario, dal nodo di destinazione al nodo di partenza
            // node = path[node] == il nodo X da cui siamo arrivati a node
            for(T node = dst; node != src; node = path[node])
                reconstructed_path.insert(reconstructed_path.begin(), node);

            reconstructed_path.insert(reconstructed_path.begin(), src);

            return reconstructed_path;

            /**
             * Figo! Ma, praticamente, come funziona il DFS?
             * La metodologia con la quale il DFS visita i nodi e' scritta
             * letteralmente nel nome: Depth First Search.
             * 
             * Quindi, cerchera' sempre di andare il piu' in profondita' possibile fino 
             * a quando non trovera' un nodo senza archi uscenti.
            */
        }

        /**
         * @brief Esegue una BFS (Breadth First Search) dal nodo di partenza
         * 
         * @param src Nodo di partenza
         * @param dst Nodo di destinazione
         * 
         * @return Ritorna un vettore contenente il primo percorso trovato, l'ultimo elemento e' 
         *         il costo totale del percorso
        */
        vector<T> BFS(const T& src, const T& dst) {
            // path: contiene il nodo precedente di ogni nodo visitato
            vector<T> path(this->adjacency_list.size(), numeric_limits<T>::max());
            vector<bool> visited(this->adjacency_list.size(), false);

            // to_visit: struttura FIFO (First In First Out)
            queue<T> to_visit;

            // Ovviamente partiamo dal nodo di partenza, no?
            to_visit.push(src);

            while(!to_visit.empty()) {
                // Prendiamo l'elemento piu' recentemente aggiunto
                T current = to_visit.front();
                to_visit.pop();

                // Se abbiamo raggiunto il nodo di destinazione, interrompiamo
                if(current == dst) break;

                // Se un nodo e' gia' stato visitato non lo visitiamo di nuovo
                if(visited[current]) continue;

                // Senno', lo marchiamo come visitato
                visited[current] = true;

                // Dopodiche', aggiungiamo tutti i nodi adiacenti a quelli visitati
                for(auto& n : this->adjacency_list[current]) {
                    if(!visited[n]) {
                        to_visit.push(n);

                        // Memorizziamo il nodo precedente per poter ricostruire il percorso
                        // n -> nodo che stiamo per visitare da current
                        path[n] = current;
                    }
                }                
            }
            
            vector<T> reconstructed_path;

            // Andiamo al contrario, dal nodo di destinazione al nodo di partenza
            // node = path[node] == il nodo X da cui siamo arrivati a node
            for(T node = dst; node != src; node = path[node])
                reconstructed_path.insert(reconstructed_path.begin(), node);

            reconstructed_path.insert(reconstructed_path.begin(), src);

            return reconstructed_path;

            /**
             * Figo! Ma, praticamente, come funziona il BFS?
             * La metodologia con la quale il BFS visita i nodi e' scritta
             * letteralmente nel nome: Breadth First Search.
             * Breadth = Ampiezza
             * 
             * Ampiezza nel senso che la BFS visitera' tutti i nodi adiacenti
             * prima di andare piu' in profondita'.
            */
        }

        /**
         * @brief Trova il percorso piu' breve tra due nodi
         * 
         * @param src Nodo di partenza
         * @param dst Nodo di destinazione
         * 
         * @return Ritorna un vettore contenente il primo percorso trovato
        */
        pair<T, vector<T>> dijkstra(const T& src, const T& dst) {
            // path: contiene il nodo precedente di ogni nodo visitato
            vector<T> path(this->adjacency_list.size());
            vector<bool> visited(this->adjacency_list.size(), false);

            // costs: contiene il costo di ogni nodo visitato dal nodo di partenza
            vector<T> costs(this->adjacency_list.size(), numeric_limits<T>::max());

            // Ovviamente, il costo del nodo di partenza al nodo di partenza e' 0 bruh
            costs[src] = 0;

            // priority_queue: coda con priorita', dove la priorita' e' la distanza piu' piccola 
            //                 dal nodo di partenza
            priority_queue<pair<T, T>, vector<pair<T, T>>, greater<pair<T, T>>> priority_queue;
            /**
             * Piccola delucidazione su come l'abbiamo creata:
             * Cosa fa il primo parametro: Immagazzina i dati tramite delle coppie di valori, 
             *                             dove il primo valore e' la distanza dal nodo di partenza
             *                             e il secondo elemento e' l'indice del nodo.
             * 
             * Cosa fa il secondo parametro: E' il container utilizzato per immagazzinare i dati
             * 
             * Cosa fa il terzo parametro: E' la funzione di comparazione che viene utilizzata per
             *                             stabilire quale elemento ha la priorita' maggiore.
             *                             In questo caso, abbiamo scelto di utilizzare la funzione
             *                             "greater" che, come il nome suggerisce, ordina i dati
             *                             in ordine crescente. (per cui il primo elemento sara'
             *                             quello con la distanza minore dal nodo di partenza)
            */

            // Ovviamente partiamo dal nodo di partenza, no?
            priority_queue.push({0, src});

            while(!priority_queue.empty()) {
                // Ci prendiamo l'elemento con il costo minore dal nodo di partenza
                auto [current_cost, current_node] = priority_queue.top();
                priority_queue.pop();

                // Se abbiamo raggiunto il nodo di destinazione, interrompiamo in quanto
                // non ci interessa trovare altri percorsi (ed anche perche' abbiamo gia'
                // trovato il percorso piu' breve grazie alla coda con priorita')
                if(current_node == dst) break;

                // Evitiamo di ricalcolare le distanze dei nodi gia' visitati
                if(visited[current_node]) continue;

                visited[current_node] = true;

                // Nel caso la distanza fosse stata aggiornata dopo che il nodo e' stato inserito
                // nella coda, non lo consideriamo in quanto significa che non e' piu' il nodo
                // con la distanza minore dal nodo di partenza
                if(current_cost != costs[current_node]) continue;

                // Iteriamo su tutti i nodi adiacenti a quello corrente
                for(size_t i = 0; i < this->adjacency_list[current_node].size(); i++) {
                    T adjacent_node = this->adjacency_list[current_node][i];
                    T adjacent_cost = this->weights[current_node][i];

                    // Se il costo dal nodo di partenza al nodo attuale + il costo da current_node a adjacent_node
                    // e' minore del costo attuale di adjacent_node allora aggiorniamo il costo di
                    // adjacent_node e lo inseriamo nella coda con priorita'
                    if(costs[current_node] + adjacent_cost < costs[adjacent_node]) {
                        costs[adjacent_node] = costs[current_node] + adjacent_cost;
                        priority_queue.push({costs[adjacent_node], adjacent_node});

                        // Siccome il nodo corrente e' (attualmente) il nodo con la distanza minore
                        // dal nodo di partenza, allora il nodo precedente di adjacent_node e' current_node
                        path[adjacent_node] = current_node;
                    }
                }
            }

            vector<T> reconstructed_path;

            // Andiamo al contrario, dal nodo di destinazione al nodo di partenza
            // node = path[node] == il nodo X da cui siamo arrivati a node
            for(T node = dst; node != src; node = path[node])
                reconstructed_path.insert(reconstructed_path.begin(), node);

            reconstructed_path.insert(reconstructed_path.begin(), src);

            return make_pair(costs[dst], reconstructed_path);

            /**
             * Minchia mbare roba di lusso, ma me lo fai un riassuntino?
             * 1. Inizialmente, si imposta la distanza del nodo di partenza a zero 
             *    e tutte le altre nodi a infinito (o al numero massimo, parlando in termini pratici).
             * 
             * 2. Incominciamo a visitare i nodi partendo dal nodo di partenza.
             * 
             * 3. Si seleziona il nodo con la distanza minima (lo facciamo attraverso la priority_queue) 
             *    dal nodo di partenza e si esaminano tutti i suoi nodi adiacenti. 
             *    Se la distanza dal nodo di partenza al nodo corrente + la distanza dal nodo corrente
             *    al nodo adiacente e' minore della distanza attuale dal nodo di partenza al nodo adiacente, 
             *    allora aggiorniamo la distanza dal nodo di partenza al nodo adiacente e inseriamo il nodo
             *  
             * 4. Ripetiamo il passaggio 3 finche' non abbiamo visitato tutti i nodi o finche' non abbiamo
             *    raggiunto il nodo di destinazione.)
             * 
             * NOTA: E' possibile che un nodo non sia raggiungibile dal nodo di partenza, in tal caso
             *       la distanza di quel nodo sara' infinita (o il numero massimo, sempre in termini pratici).
            */
        }
};

int main() {
    Grafo<size_t> grafo(6);

    // E' il grafo che si trova all'inizio del file
    grafo.add_edge(0, 1, 7);
    grafo.add_edge(0, 5, 14);
    grafo.add_edge(1, 2, 10);
    grafo.add_edge(1, 3, 20);
    grafo.add_edge(2, 3, 11);
    grafo.add_edge(2, 5, 1);
    grafo.add_edge(2, 4, 3);
    grafo.add_edge(4, 5, 9);

    cout << "DFS from 0 to 3: ";
    for(auto& p : grafo.DFS(0, 3)) {
        cout << p << " ";
    }
    cout << endl;

    cout << "BFS from 0 to 3: ";
    for(auto& p : grafo.BFS(0, 3)) {
        cout << p << " ";
    }
    cout << endl;

    auto[cost, path] = grafo.dijkstra(0, 3);
    cout << "Dijkstra from 0 to 3: ";
    for(auto& p : path) {
        cout << p << " ";
    }
    cout << "(cost: " << cost << ")" << endl;

    return 0;
}