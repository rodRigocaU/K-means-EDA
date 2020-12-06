#include <iostream>
#include<time.h>
#include<sstream>
#include<fstream>
#include<cmath>
#include<vector>
#include<string>
#include<SFML/Graphics.hpp>
#include<SFML/Window.hpp>

using namespace std;
double INFI = 1000000000.0;
 
//PARA LOS PUNTOS DENTRO DE NUESTRO ESPACIO 
class point 
{
    public:
        double distan;
        int cluKind;
        double  pckp_lgd, pckp_ltd; //NUESTRAS COORDENADAS EN EL ESPACIOS
           
        /*point() 
        {
            pckp_lgd = 0.0;
            pckp_ltd = 0.0;
            cluKind = 0;//0 SIGNIFICA QUE NO PERTENECE A NINGUN CLUSTER POR EL MOMENTO
            distan = INFI;//ES LA DISTANCIA MINIMA HACIA EL CLUSTER QUE PERTENECE ACTUALMENTE, AYUDA MUCHO DESPUES DE ORDENAR LOS CENTROIDES
            //Y QUEREMOS VERIFICAR SI ES QUE LA DISTANCIA ES MENOR CON OTRO CLUSTER Y LO CAMBIAMOS SI ES QUE OCURRE
        }*/
        point(double pckp_lgd2, double pckp_ltd2)
        {
            pckp_lgd = pckp_lgd2;
            pckp_ltd = pckp_ltd2;
            cluKind = -1000;//-1000 SIGNIFICA QUE NO PERTENECE A NINGUN CLUSTER POR EL MOMENTO
            distan = INFI;//ES LA DISTANCIA MINIMA HACIA EL CLUSTER QUE PERTENECE ACTUALMENTE, AYUDA MUCHO DESPUES DE ORDENAR LOS CENTROIDES
            //Y QUEREMOS VERIFICAR SI ES QUE LA DISTANCIA ES MENOR CON OTRO CLUSTER Y LO CAMBIAMOS SI ES QUE OCURRE (POR EL MOMENTO ES INIFITO)
        }
        double euDis(point x)
        {
            //EL EJE X pckp_lgd EL EJE Y pckp_ltd
            return sqrt((x.pckp_lgd - pckp_lgd) * (x.pckp_lgd - pckp_lgd) + (x.pckp_ltd - pckp_ltd) * (x.pckp_ltd - pckp_ltd));
        }
};


//NUESTRO VECTOR DE PUNTOS, EL NUMERO DE ITERACIONES Y EL NUMERO DE CLUSTERES
void kmeans(vector<point>* pntos, int itera, int k)
{
   //ESCOGEMOS LOS PUNTOS RANDOMS PARA LOS CLUSTER
    vector<point> centros;
    srand(time(NULL));
    int n = pntos->size();
    //ESCOGEMOS LOS CLUSTER EN PUNTOS RANDOM DE NUESTRO ESPACIO
    for (int i = 0; i < k; ++i) 
    {
        centros.push_back((*pntos)[rand() % n]);
    }
    //BUCLE INFINITO HASTA QUE LOS CENTROIDES ESTEN EN SU MEJOR POSICION
    //ITERA = 1 => SI CUMPLE QUE LOS CENTROIDES ESTAN EN LA MEJOR POS ENTONCES ITERA = 0
    while (itera)
    {
        //AGRUPEMOS CADA PUNTO CON EL CLUSTER CORRESPONDIENTE
        for (int i = 0; i < centros.size(); i++)
        {
            for (int j = 0; j < (*pntos).size(); j++)
            {
                double aux;
                aux = centros[i].euDis((*pntos)[j]);
                //SI ES QUE LA DISTANCIA ES MENOR A LA DISTANCIA QUE TENIAMOS AL ANTERIOR CLUSTER ENTONCES SOLO REEMPLAZAMOS 
                if (aux < (*pntos)[j].distan)
                {
                    (*pntos)[j].distan = aux;
                    (*pntos)[j].cluKind = i;

                }
            }
        }
        //PARA RE CALCULAR LOS CENTROIDES UNA MANERA SENCILLA SERA LA MEDIA DE LOS PUNTOS x e y
        vector<int> nroP;
        vector<double> totalLGD,totalLTD;
        //COMENZAMOS EN CERO TODO PARA HACER LAS RESPECTIVAS SUMAS
        for (int j = 0; j < k; ++j) 
        {
            nroP.push_back(0);
            totalLGD.push_back(0.0);
            totalLTD.push_back(0.0);
        }
        //CLASIFICAMOS POR CADA CLUSTER Y REINICIAMOS LAS DISTANCIA DE LOS PUNTOS PARA QUE SEA MAS SENCILLO VOLVER A CALCULAR A QUE CLUSTER PERTENECE
        //DESPUES DE RECALCULAR LOS CENTROIDES DE CADA CLUSTER
        for (int i = 0; i < (*pntos).size(); i++)
        {
            nroP[(*pntos)[i].cluKind]++;
            totalLGD[(*pntos)[i].cluKind] += (*pntos)[i].pckp_lgd;
            totalLTD[(*pntos)[i].cluKind] += (*pntos)[i].pckp_ltd;
            (*pntos)[i].distan = INFI;
        }
        
 

        // RECALCULAMOS LOS NUEVOS CENTROIDES
        int cont = 0; 
        for (int i = 0; i < centros.size(); i++)
        {
            double aux1, aux2;
            aux1 = centros[i].pckp_lgd;
            aux2 = centros[i].pckp_ltd;

            //DESPUES DE TENER TONAL DIVIDIMOS PARA OBTENER LA MEJOR POSICION DEL CENTRO DEL CLUSTER
            
            centros[i].pckp_lgd = totalLGD[i] / nroP[i];
            centros[i].pckp_ltd = totalLTD[i] / nroP[i];

            //SI ES QUE NO HA OCURRIDO UN CAMBIO DE POS DEL CENTROIDE SIGNIFICA QUE EL CENTROIDE ESTA EN LA
            //MEJOR POSICION DEPENDIENDO DE SU CLUSTER POR ESO AUMENTAMOS EN 1 EL CONT
            if (aux1 == centros[i].pckp_lgd && aux2 == centros[i].pckp_ltd)
            {
                ++cont;
            }
        }
        //LA CONDICION DE PARADA QUE TODOS LOS CLUSTER ESTAN EN SU MEJOR POSICION ES QUE 
        //EL CONT == K EL NUMERO DE CLUSTER QUE TENEMOS
        if (cont >= k)
        {
            itera = 0;
        }

    }
   

}

vector<point> readdataset()
{
    vector<point> pntos;
    string line;
    ifstream file("sample.csv");
    for (int i = 0; i < 100000; i++)
    {
        getline(file, line);
        stringstream lines(line);

        string col;

        double pckp_lgd, pckp_ltd;

        getline(lines, col, ',');
        getline(lines, col, ',');
        getline(lines, col, ',');
       
        getline(lines, col, ',');
        getline(lines, col, ',');
        getline(lines, col, ',');
        
        stringstream ss;
        ss << col;
        ss >> pckp_lgd;
        //PARA VER MEJOR LOS GRAFICOS MULTIPLICAR X7
        //PARA VER EL ORIGINAL COMENTAMOS LA LINEA SIGUIENTE
        pckp_lgd *= 7;
        
        //cout << pckp_lgd << '\n';

        getline(lines, col, ',');

        stringstream ss2;
        ss2 << col;
        ss2 >> pckp_ltd;
        pckp_ltd *= 7;

        //cout << pckp_ltd << '\n';
        pntos.push_back(point(pckp_lgd, pckp_ltd));
    }
    return pntos;
    
}


int main()
{

    //CLUSTERING
    srand(time(NULL));
    vector<point> ps;
    //LLENADO CON LOS PUNTOS DEL DATASET
    ps = readdataset();
    //LLENADO CON PUNTOS RANDOM 
    /*for (int i = 0; i < 90; i++)
    {
        ps.push_back(point(50 * (1.0 + rand() % (401 - 1))/80, 50 * (1.0 + rand() % (401 - 1))/ 80));
    }*/
    //cout << ps.size() << '\n';
    kmeans(&ps, 1, 6);


    //DIBUJAR CLUSTERS
    sf::RenderWindow window(sf::VideoMode(400, 400), "Clustering");
    sf::View view1;

    //view1.zoom(1.2f);
    //CENTRO ORIGINAL Y QUITAMOS LA MULTIPLICACION POR 7 AL RECUPERAR LOS DATOS 
    //view1.setCenter(-70.f, 40.f);
    //SI QUIEREMOS VER LOS PUNTOS EN 0
    //view1.setCenter(0.f, 0.f);
    //CENTRO CON LOS PUNTOS X7
    view1.setCenter(-490.f, 350.f);

    //PRUEBAS
   /* sf::CircleShape circle(5);
    circle.setFillColor(sf::Color::Black);
    circle.setPosition(sf::Vector2f(-70, 40));

    sf::CircleShape circle2(5);
    circle2.setFillColor(sf::Color::Red);
    circle2.setPosition(sf::Vector2f(80, 40));*/


    //USAREMOS LOS CIRCULOS PARA REPRESENTAR A LOS PUNTOS Y A CADA CIRCULO SU POSICION RESPECTIVA
    //Y TAMBIEN A CADA PUNTO SU COLOR DEL CLUSTER
    vector<sf::CircleShape> circles;
    //ESTO ES PARA COLOREAR CADA CLUSTER EN ESTE CASO TENEMOS UN TOTAL DE 6 CLUSTER POR ESO HAY 6 COLORES
    //SI NOSOTROS NECESITAMOS MAS CLUSTERS UNICAMENTE AUMENTAMOS NUESTROS COLORES EN NUESTRO ARREGLO
    sf::Color colores[6] = { sf::Color::Blue,sf::Color::Magenta,sf::Color::Red,sf::Color::Green,sf::Color::Black,sf::Color::Yellow };
    for (int i = 0; i < ps.size(); i++)
    {
        //PARA LOS RANDOM CAMBIAMOS EL RADIO DE 0.3 A 1.6
        circles.push_back(sf::CircleShape(0.3));
    }
    for (int i = 0; i < ps.size(); i++)
    {
        circles[i].setPosition(sf::Vector2f(ps[i].pckp_lgd, ps[i].pckp_ltd));
        circles[i].setFillColor(colores[ps[i].cluKind]);
    }

    
    cout << circles.size() << '\n';

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            //PARA HACER ZOOM CON LA RUEDA DEL RATON
            if (event.type == sf::Event::MouseWheelMoved)
            {
                view1.zoom(1.f + event.mouseWheel.delta * 0.05f);
            }
            if (event.type == sf::Event::Closed)
                window.close();
            
        }
        //SI QUEREMOS USAR EL RANDOM COMENTAMOS ESTE VIEW
        window.setView(view1);
        window.clear(sf::Color::White);
        //DIBUJAMOS CADA CIRCULO
        for (int i = 0; i < circles.size(); i++)
        {
            window.draw(circles[i]);
        }
        //window.draw(circle);
        //window.draw(circle2);
        //sf::Vertex pointt(sf::Vector2f(700, 500), sf::Color::White);
        //window.draw(&pointt, 1, sf::Points);
        window.display();
        
    }
    
    return 0;

}
