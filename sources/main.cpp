#include <iostream>
#include <prefixtree.h>
#include <filestorage.h>

using namespace std;

int main()
{
    //system("rm -f ./database.bin");
    FileStorage storage(64, "./database.bin");
    storage.init();
    
    PrefixTree<string> p(&storage, [](string v){return v;}, [](string v){return v;});
    p.set("this is a test", "aaa this is a test  this is a test  this is a test  this is a test  this is a test  this is a test  this is a test  this is a test  this is a test  this is a test  this is a test  this is a test  this is a test  this is a test  this is a test  this is a test  this is a test  this is a test  this is a test  this is a test  this is a test  this is a test  this is a test  this is a test  this is a test  this is a test  this is a test  this is a test  this is a test  this is a test  this is a test  this is a test  this is a test  this is a test  this is a test  this is a test  this is a test  this is a test  this is a test  this is a test  this is a test  this is a test  this is a test  this is a test  this is a test  this is a test  this is a test  this is a test  this is a test  this is a test  this is a test  this is a test  this is a test  this is a test  this is a test  this is a test  this is a test  this is a test  this is a test  this is a test  this is a test  this is a test  this is a test bbbbbb");
    auto ret = p.get("this is a test");

    cout << "returned value: " << ret << endl;

    map<string, string> m;
    m["this is a test"] = "the value";
    m["bola"] = "bola";
    m["bola2"] = "bola2";
    m["bolo"] = "bola3";
    m["bolé"] = "bola4";

    for (auto &it : m)
    {
        p.set(it.first, it.second);
    }

    for (auto &it : m)
    {
        auto ret = p.get(it.first);
        cout << "returned value (key '" << it.first <<"'): " << ret << endl;
    }

    int d = 0;
    int e = 0;
    for (int a =0; a < 20; a++)
    {
        for (int c = 0; c < 10; c++)
        {
            p.set("key" + to_string(d), "value" + to_string(d));
            d++;
        }

        for (int c = 0; c < 10; c++)
        {
            auto ret = p.get("key" + to_string(e));
            cout << "returned value (key 'key" << e <<"'): " << ret << endl;
            e++;
        }
    }

    vector<string> keys = { "O", "TCP/IP", "foi", "desenvolvido", "em", "1969", "pelo", "U.S.", "Department", "of", "Defense", "Advanced", "Research", "Projects", "Agency,", "como", "um", "recurso", "para", "um", "projeto", "experimental", "chamado", "de", "ARPANET", "(Advanced", "Research", "Project", "Agency", "Network)", "para", "preencher", "a", "necessidade", "de", "comunicação", "entre", "um", "grande", "número", "de", "sistemas", "de", "computadores", "e", "várias", "organizações", "militares", "dispersas.", "O", "objetivo", "do", "projeto", "era", "disponibilizar", "links", "(vínculos)", "de", "comunicação", "com", "alta", "velocidade,", "utilizando", "redes", "de", "comutação", "de", "pacotes.", "O", "protocolo", "deveria", "ser", "capaz", "de", "identificar", "e", "encontrar", "a", "melhor", "rota", "possível", "entre", "dois", "sites", "(locais),", "além", "de", "ser", "capaz", "de", "procurar", "rotas", "alternativas", "para", "chegar", "ao", "destino,", "caso", "qualquer" };
    vector<string> values = {"uma", "das", "rotas", "tivesse", "sido", "destruída.", "O", "objetivo", "principal", "da", "elaboração", "de", "TCP/IP", "foi", "na", "época,", "encontrar", "um", "protocolo", "que", "pudesse", "tentar", "de", "todas", "as", "formas", "uma", "comunicação", "caso", "ocorresse", "uma", "guerra", "nuclear.", "A", "partir", "de", "1972", "o", "projeto", "ARPANET", "começou", "a", "crescer", "em", "uma", "comunidade", "internacional", "e", "hoje", "se", "transformou", "no", "que", "conhecemos", "como", "Internet.", "Em", "1983", "ficou", "definido", "que", "todos", "os", "computadores", "conectados", "ao", "ARPANET", "passariam", "a", "utilizar", "o", "TCP/IP,", "pois", "na", "época", "o", "rádio", "e", "a", "comunicação", "por", "satélite", "fora", "introduzida,", "e", "o", "modelo", "seguido", "pela", "ARPANET", "até", "então", "dificultava", "a", "integração", "dela", "com", "essas", "tecnologias", "emergentes[2].", "No", "final", "dos", "anos", "1980", "a", "Fundação", "Nacional", "de", "Ciências", "em", "Washington,", "D.C.,", "começou", "a", "construir", "o", "NSFNET,", "um", "backbone", "para", "um", "supercomputador", "que", "serviria", "para", "interconectar", "diferentes", "comunidades", "de", "pesquisa", "e", "também", "os", "computadores", "da", "ARPANET.", "Em", "1990", "o", "NSFNET", "se", "tornou", "o", "backbone", "das", "redes", "para", "a", "Internet,", "padronizando", "definitivamente", "o", "TCP/IP.[3]"};

    for (int a = 0; a < keys.size(); a++)
    {
        cout << "setting key '" << keys[a] << "' with value '" << values[a] << "'" << endl;
        p.set(keys[a], values[a]);
    }

    for (int a = 0; a < keys.size(); a++)
    {
        auto ret = p.get(keys[a]);
        cout << "returned value (key '" << keys[a] <<"'): " << ret << endl;
    }

    return 0;
}