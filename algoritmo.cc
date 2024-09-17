#include <bits/stdc++.h>
#define nd '\n'
#define ll long long
#define b6 bitset<6>
#define b4 bitset<4>
using namespace std;

//definir valores iniciales
const int cantKeys = 100;
const ll maxN = 1e9+7;
const ll initialS = 77777;
const int limPrimos = 10000;

//generacion de nodesID
vector <b6> availNodes (62);
void setIds() {for (int i=1; i<63; i++) availNodes[i] = i;}
//generacion de primos
vector <int> primos;
bitset<limPrimos> compuestos;
void generatePrimes(){

	//utilizamos la tecnica de la criba de eratostenes para generar primos
	for (int i=2; i<limPrimos; i++){

		if (!compuestos[i]){
			primos.push_back(i);
			for (int k=i*2; k<limPrimos; k+=i) compuestos[k] = 1;
		}
	}
}

string enteros (string s){
    
    string final = "";
    
    for (ll i=0; i<(int)s.size(); i++){
        
        int v = s[i];
        
        string num = to_string(v);
        
        while(num.size() < 3) num = '0'+num;
        
        final+=num;
    }
    
    return final;
}

string recuperar (string s){
    
    string final = "";
    
    for (int i=0; i<(int)s.size(); i+=3){
        
        string num = s.substr(i, 3);
        
        int v = stoi(num);
        
        char c = v;
        
        final+=c;
    }
    
    return final;
}

void inicialization (){

	setIds();
	generatePrimes();
}

struct node {

	ll keys[cantKeys]; //llaves
	b6 id; //nodeId
	b6 idPar; //nodeId en el par
	ll s; //semilla
	int psn; //PSN
	ll p; //primer primo
	ll q; //segundo primo
    int indexKey=0; //indice de la llave
    string encrypted="";

	node (){}

	node(b6 Id, b6 IdPar, int Psn){
		id = Id;
		idPar = IdPar;
		s = initialS;
		psn = Psn;
	}
	ll fs (ll a){return (((a%maxN)*(s%maxN))%maxN);}; //scrambled function
	ll fg (ll a, ll b){return (((((a%maxN)*(b%maxN))%maxN)+b)%maxN);} //generation function
	void fm (ll a){s = (((((a%maxN)*(a%maxN))%maxN)*(s%maxN))%maxN);} //mutation function

	//algoritmo de generacion de llaves
	void genKeys (){

		for (int i=0; i<cantKeys; i+=2){

			ll p0 = fs(p);
			ll k = fg(p0, q);
			fm(q);

			keys[i] = k;

			ll q0 = fs(q);
			k = fg(q0, p);
			fm(p0);

			keys[i+1] = k;
		}
		
		cout << nd;
		
		cout << "Nueva tabla de llaves generada" << nd;

		for (int i=0; i<cantKeys; i++) cout << keys[i] << " ";
		cout << nd << nd;
	}
};

map <int, node> nodos;

//Encriptamiento cesar

string f1 (string s, ll key){
    
    int n = s.size();
    
    for (int i=0; i<n; i++){
        
        s[i] = (s[i]+key)%128;
    }
    
    return s;
}

string f1_decrypt (string s, ll key){
    
    int n = s.size();
    
    for (int i=0; i<n; i++){
        
        s[i] = (s[i]-(key%128)+128)%128;
    }
    
    return s;
}

string f2 (string s, ll key){
    
    int n = s.size();
    
    for (int i=0; i<n; i++){
        
        s[i] = (s[i]^key)%128;
    }
    
    return s;
}

string f2_decrypt (string s, ll key){
    
    int n = s.size();
    
    for (int i=0; i<n; i++){
        
        s[i] = (s[i]^key)%128;
    }
    
    return s;
}

string f3 (string s, ll key){
    
    reverse(s.begin(), s.end());
    
    int n = s.size();
    
    for (int i=0; i<n; i++){
        
        s[i] = (s[i]+key)%128;
    }
    
    return s;
}

string f3_decrypt (string s, ll key){
    
    reverse(s.begin(), s.end());
    
    int n = s.size();
    
    for (int i=0; i<n; i++){
        
        s[i] = (s[i]-(key%128)+128)%128;
    }
    
    return s;
}

string f4 (string s, ll key){
    
    reverse(s.begin(), s.end());
    
    int n = s.size();
    
    for (int i=0; i<n; i++){
        
        s[i] = (s[i]^key)%128;
    }
    
    return s;
}

string f4_decrypt (string s, ll key){
    
    reverse(s.begin(), s.end());
    
    int n = s.size();
    
    for (int i=0; i<n; i++){
        
        s[i] = (s[i]^key)%128;
    }
    
    return s;
}

b6 getID (){

	//si no hay nodeID disponible retornamos 0
	b6 id (0);
	if (availNodes.empty()) return id;

	//sino tomamos un indice random, lo eliminamos y retornamos
	else {

		int randIndex = rand() % (int)availNodes.size();
		swap(availNodes[randIndex], availNodes[availNodes.size()-1]);
		availNodes.pop_back();
		return availNodes[randIndex];
	}
}

string FCM_encrypt (){
    
    cout << "---------Encriptacion ECM-------------" << nd << nd;

	b6 nodeId = getID(); //obtenemos un id disponible
	
	cout << "Indice aleatorio escogido: " << nodeId << nd;

	//creamos un nuevo nodo
	nodos[(int)nodeId.to_ulong()] = node(nodeId, 0, 0);

	//generamos los primos
	nodos[(int)nodeId.to_ulong()].p = rand()%primos.size();
	nodos[(int)nodeId.to_ulong()].q = rand()%primos.size();
	
	cout << "primos generados: " << nodos[(int)nodeId.to_ulong()].p << " " << nodos[(int)nodeId.to_ulong()].q << nd;

	//generamos el conjunto de llaves
	nodos[(int)nodeId.to_ulong()].genKeys();
	

	nodos[(int)nodeId.to_ulong()].encrypted = enteros(nodeId.to_string()+'/' +"FCM"+'/'+to_string(nodos[(int)nodeId.to_ulong()].p)+'-'+ to_string(nodos[(int)nodeId.to_ulong()].q) + '/');

    return nodos[(int)nodeId.to_ulong()].encrypted;
}

string KUM_encrypt (b6 nodeId){
    
    cout << "----------------Encriptacion KUM ------------------" << nd << nd;

	//generamos los primos
	nodos[(int)nodeId.to_ulong()].p = rand()%primos.size();
	nodos[(int)nodeId.to_ulong()].q = rand()%primos.size();

	//generamos el conjunto de llaves
	nodos[(int)nodeId.to_ulong()].genKeys();

	nodos[(int)nodeId.to_ulong()].encrypted = enteros(nodos[(int)nodeId.to_ulong()].idPar.to_string()+'/' +"KUM"+'/'+to_string(nodos[(int)nodeId.to_ulong()].p)+'-'+ to_string(nodos[(int)nodeId.to_ulong()].q)+'/');

    return nodos[(int)nodeId.to_ulong()].encrypted;
}


string RM_encrypt (b6 nodeId, string message){
    
    cout << "----------------Encriptacion RM ------------------" << nd << nd;

    int t = message.size();
    
    b4 psn = ((int)message[t-1]+(int)message[t-2])%16;
    
    cout << "psn generado: " << psn << nd; 

    while (message.size()%4 != 0) message = '0'+message;
    
    cout << "mensaje modificado: " << message << nd;

    int cortes = message.size()/4;
    
    cout << "el mensaje se dividira en " << cortes << " partes" << nd;

    string partes [4];
    int funNum;

    string message2="";
    
    cout << nd << "Mensaje dividido: " << nd;
    
    cout << "Llave utilizada: " << (int)nodos[(int)nodeId.to_ulong()].keys[nodos[(int)nodeId.to_ulong()].indexKey] << nd;

    for (int i=0; i<4; i++){
        

        partes[i] = message.substr(i*cortes, cortes);

        funNum = ((int)psn.to_ulong()*i)%4;
        
        cout << partes[i] << " ";

        if (funNum == 0) partes[i] = f4(partes[i], (int)nodos[(int)nodeId.to_ulong()].keys[nodos[(int)nodeId.to_ulong()].indexKey]);
        if (funNum == 1) partes[i] = f1(partes[i], (int)nodos[(int)nodeId.to_ulong()].keys[nodos[(int)nodeId.to_ulong()].indexKey]);
        if (funNum == 2) partes[i] = f2(partes[i], (int)nodos[(int)nodeId.to_ulong()].keys[nodos[(int)nodeId.to_ulong()].indexKey]);
        if (funNum == 3) partes[i] = f3(partes[i], (int)nodos[(int)nodeId.to_ulong()].keys[nodos[(int)nodeId.to_ulong()].indexKey]);
    
        message2+=partes[i];
    }
    
    cout << message2.size() << nd;

    nodos[(int)nodeId.to_ulong()].indexKey = (nodos[(int)nodeId.to_ulong()].indexKey+1)%cantKeys;

    nodos[(int)nodeId.to_ulong()].encrypted = enteros(nodos[(int)nodeId.to_ulong()].idPar.to_string()+'/'+"RM"+'/'+message2+'/'+psn.to_string());
    
    cout << nd << "Mensaje encriptado: //// " << nodos[(int)nodeId.to_ulong()].encrypted << " ////" << nd;
    
    return nodos[(int)nodeId.to_ulong()].encrypted;
}

string LCM_encrypt (b6 idNodo){
    
    cout << "-------------Encriptacion LCM-------------------" << nd << nd;
    
    availNodes.push_back(idNodo);
    
    cout << "Nodo eliminado exitosamente " << nd;
    
    string r = enteros(nodos[(int)idNodo.to_ulong()].idPar.to_string()+'/'+"LCM"+'/'+'/');
    
    return r;
}

b6 FCM_decrypt (b6 idExt, string message){
    
    cout << "Tipo: FCM" << nd;

	b6 nodeId = getID(); //obtenemos un id disponible
	
	cout << "Indice de nodo asignado: " << nodeId << nd;

	//creamos un nuevo nodo
	nodos[(int)nodeId.to_ulong()] = node(nodeId, idExt, 0);

	//extraemos los numeros primos en el mensaje
	string ps = "";
	string qs = "";

	int i;
	
	cout << message << nd;

	for (i=0; i<(int)message.size(); i++){

		if (message[i] == '-') break;
		ps.push_back(message[i]);

	}

	for (i+=1; i<(int)message.size(); i++){
		qs.push_back(message[i]);
	}

	nodos[(int)nodeId.to_ulong()].p = stoi(ps);
	nodos[(int)nodeId.to_ulong()].q = stoi(qs);
	
	cout << "Primos: " << nodos[(int)nodeId.to_ulong()].p << " " << nodos[(int)nodeId.to_ulong()].q << nd;

	//generamos el conjunto de llaves
	nodos[(int)nodeId.to_ulong()].genKeys();
	
	cout << "Nuevo elemento guardado exitosamente" << nd;
	
	RM_encrypt(nodeId, nodeId.to_string());
	
	return nodeId;
}

void KUM_decrypt (b6 nodeId, string message){
    
    cout << "Tipo KUM" << nd;

	//extraemos los numeros primos en el mensaje
	string ps = "";
	string qs = "";

	int i;

	for (i=0; i<(int)message.size(); i++){

		if (message[i] == '-') break;
		ps.push_back(message[i]);

	}

	for (i+=1; i<(int)message.size(); i++){
		qs.push_back(message[i]);
	}

	nodos[(int)nodeId.to_ulong()].p = stoi(ps);
	nodos[(int)nodeId.to_ulong()].q = stoi(qs);

	//generamos el conjunto de llaves
	nodos[(int)nodeId.to_ulong()].genKeys();
}

void RM_decrypt(b6 nodeId, string message, b4 psn){
    
    cout << "Tipo RM" << nd;

    int cortes = message.size()/4;

    string partes [4];
    int funNum;
    string message2="";
    
    cout << "Mensajes encriptados: " << nd;
    
    cout << "Llave utilizada: " << (int)nodos[(int)nodeId.to_ulong()].keys[nodos[(int)nodeId.to_ulong()].indexKey] << nd;

    for (int i=0; i<4; i++){
        partes[i] = message.substr(i*cortes, cortes);
        
        cout << partes[i] << " ";

        funNum = ((int)psn.to_ulong()*i)%4;

        if (funNum == 0) partes[i] = f4_decrypt(partes[i], (int)nodos[(int)nodeId.to_ulong()].keys[nodos[(int)nodeId.to_ulong()].indexKey]);
        if (funNum == 1) partes[i] = f1_decrypt(partes[i], (int)nodos[(int)nodeId.to_ulong()].keys[nodos[(int)nodeId.to_ulong()].indexKey]);
        if (funNum == 2) partes[i] = f2_decrypt(partes[i], (int)nodos[(int)nodeId.to_ulong()].keys[nodos[(int)nodeId.to_ulong()].indexKey]);
        if (funNum == 3) partes[i] = f3_decrypt(partes[i], (int)nodos[(int)nodeId.to_ulong()].keys[nodos[(int)nodeId.to_ulong()].indexKey]);
    }
    
    cout << nd;

    nodos[(int)nodeId.to_ulong()].indexKey = (nodos[(int)nodeId.to_ulong()].indexKey+1)%cantKeys;

    string output = "";

    for (int i=0; i<4; i++) output += partes[i];
    
    cout << nd << "Mensaje final: " << output << nd;

    int inicio = 0;

    while (output[inicio] == '0') inicio++;

    output = output.substr(inicio, output.size()-inicio);

    if (nodos[(int)nodeId.to_ulong()].idPar.to_ulong() == 0){

        b6 idpar(stoi(output));
        nodos[(int)nodeId.to_ulong()].idPar = idpar;
    }

}

void LCM_decrypt (b6 idNodo){
    
    cout << "tipo LCM" << nd;
    
    availNodes.push_back(idNodo);
    
    cout << "Nodo eliminado correctamente" << nd;
}

void decrypt(string message){
    
    cout << "-------------------Desencriptando----------------------" << nd;

	string nodeId = "";

	int i=0;

	for (; i<(int)message.size(); i++){

		if (message[i] == '/') break;

		nodeId.push_back(message[i]);
	}

	string type = "";

	for (i+=1; i<(int)message.size(); i++){

		if (message[i] == '/') break;

		type.push_back(message[i]);
	}

	string content="";

	for (i+=1; i<(int)message.size(); i++){

        if (message[i] == '/') break;
		content.push_back(message[i]);
	}

    string psns="";

    for (i+=1; i<(int)message.size(); i++){

		psns.push_back(message[i]);
	}
	
	cout << "id: " << nodeId << " mensaje: " << message << nd; 
	
	b4 psn;

    if (!psns.empty()) psn = stoi(psns);

    b6 idExt (stoi(nodeId));

	if (type == "FCM") FCM_decrypt(idExt, content);
	if (type == "KUM") KUM_decrypt(idExt, content);
    if (type == "RM") RM_decrypt(idExt, content, psn) ;
    if (type == "LCM") LCM_decrypt(idExt);
}

int main() {

	srand(static_cast<unsigned int>(time(0)));
	inicialization();
	
	string operacion = "";
	string enc="";
	
	while (true){
	    
	    cout << "¿Se desea encriptar o desencriptar?" << nd;
	    cin >> enc;
	    
	    if (enc == "desencriptar") {
	        
	        string s;
	        cout << "ingrese el paquete encriptado: " << nd;
	        cin >> s;
	        decrypt(recuperar(s));
	        continue;
	    }
	    
	    cout << "¿Que operacion desea realizar?" << nd; 
	    
	    cin >> operacion;
	    
	    if (operacion == "FCM"){
	        
	        string message = FCM_encrypt();
	        cout << "Mensaje encriptado: //// " << message << " ////" << nd;
	    }
	
	    else if (operacion == "KUM") {
	        
	        cout << "Ingrese nodoId: "; 
	        
	        string nid;
	        
	        cin >> nid;
	        
	        b6 NID(stoi(nid));
	        
	        string message = KUM_encrypt(NID);
	        
	        cout << "Mensaje encriptado: //// " << message << " ////" << nd;
	    }
	    
	    else if (operacion == "RM"){
	        
	        cout << "Ingrese nodoId: "; 
	        
	        string nid;
	        
	        cin >> nid;
	        
	        b6 NID(stoi(nid));
	        
	        cout << "Ingrese el mensaje que desea encriptar: " << nd;
	        
	        string message;
	        
	        cin >> message;
	        
	        message = RM_encrypt(NID, message);
	    }
	    
	    else {
	        
	        cout << "Ingrese nodoId: "; 
	        
	        string nid;
	        
	        cin >> nid;
	        
	        b6 NID(stoi(nid));
	        
	        string message = LCM_encrypt(NID);
	        
	        cout << "Mensaje encriptado: //// " << message << " ////" << nd;
	   }
	}

	return 0;
}
