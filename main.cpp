#include <iostream>
#include <fstream>
#include <cstring>
#define STRMAX 1024

struct Pixel
{
    int r, g, b;
};

struct Imagem
{
    char type[3];              // TIPO DO ARQUIVO, IMAGENS FORMATO PPM TEM TIPO 'P3'
    int nc, nl;                // VALOR DE COLUNAS E  LINHAS DA MATRIZ
    int max;                   // VALOR MÁXIMO DE UM CANAL R, G, B PODEM ASSUMIR
    Pixel mat[STRMAX][STRMAX]; // MATRIZ COM TODOS OS PIXELS
};

using namespace std;

void abre_imagem(char nome[], Imagem &img_out);
void salva_img(char nome[], Imagem img_in);
void solariza(Imagem img_in, Imagem &img_out, int limiar);
void rotaciona_horario(Imagem img_in, Imagem &img_out);
void rotaciona_antihorario(Imagem img_in, Imagem &img_out);
void aumenta_tamanho_vmp(Imagem img_in, Imagem &img_out);
void gera_ruido(Imagem img_in, Imagem &img_out, int porcentagem);
void preencher_vetor(int vetor_aux_red[], int vetor_aux_green[], int vetor_aux_blue[], Imagem &img_in, int i, int j);
void ordenar(int vetor[], int Tam);
void remove_ruido_mediana(Imagem img_in, Imagem &img_out);

int main()
{
    char respUsuario = 'S';
    int menu;
    char nome[51], Salvar[51];

    cout << "Digite o nome da imagem que quer abrir \n";
    cin.getline(nome, 51);
    Imagem img_in, img_out;
    abre_imagem(nome, img_in);

    while (respUsuario == 'S')
    {
        char newImage;

        if (newImage == 'N')
        {
            cout << "Digite o nome da imagem que quer abrir \n";
            cin.ignore();
            cin.getline(nome, 51);
            abre_imagem(nome, img_in);
        }

        cout << "Digite o número da operação que deseja realizar na imagem: " << endl;
        cout << "| ---------------------------------------------------- |\n";
        cout << "|                   1 - SALVAR IMAGEM                  | " << endl;
        cout << "| ---------------------------------------------------- |\n";
        cout << "|                   2 - SOLARIZAÇÃO                    |" << endl;
        cout << "| ---------------------------------------------------- |\n";
        cout << "|             3 - ROTACIONAR SENTIDO HORÁRIO           |" << endl;
        cout << "| ---------------------------------------------------- |\n";
        cout << "|          4 - ROTACIONAR SENTIDO ANTI-HORÁRIO         |" << endl;
        cout << "| ---------------------------------------------------- |\n";
        cout << "|              5 - AUMENTAR POR VIZINHANÇA             |" << endl;
        cout << "| ---------------------------------------------------- |\n";
        cout << "|              6 - GERAR RUÍDO EM IMAGEM               |" << endl;
        cout << "| ---------------------------------------------------- |\n";
        cout << "|                   7 - REMOVER RUÍDO                  |" << endl;
        cout << "| ---------------------------------------------------- |\n";
        cout << "\n\n";
        cin >> menu;
        cin.ignore();

        if (menu == 1)
        {
            cout << "Escreva como quer salvar a imagem\n";
            cin >> Salvar;
            salva_img(Salvar, img_in);
        }

        else if (menu == 2)
        {
            int limiar;
            cout << "Escreva como quer salvar a imagem\n";
            cin.getline(Salvar, 51);
            salva_img(Salvar, img_in);
            cout << "Digite o limiar para solarização\n";
            cin >> limiar;
            solariza(img_in, img_out, limiar);
            salva_img(Salvar, img_out);
        }
        else if (menu == 3)
        {
            cout << "Escreva o nome de como quer salvar a imagem\n";
            cin.getline(Salvar, 51);
            salva_img(Salvar, img_in);
            rotaciona_horario(img_in, img_out);
            salva_img(Salvar, img_out);
        }
        else if (menu == 4)
        {
            cout << "Escreva o nome de como quer salvar a imagem\n";
            cin.getline(Salvar, 51);
            salva_img(Salvar, img_in);
            rotaciona_antihorario(img_in, img_out);
            salva_img(Salvar, img_out);
        }
        else if (menu == 5)
        {
            cout << "Escreva o nome de como quer salvar a imagem\n";
            cin.getline(Salvar, 51);
            salva_img(Salvar, img_in);
            aumenta_tamanho_vmp(img_in, img_out);
            salva_img(Salvar, img_out);
        }
        else if (menu == 6)
        {
            int porcentagem;
            cout << "Escreva o nome de como quer salvar a imagem\n";
            cin.getline(Salvar, 51);
            salva_img(Salvar, img_in);
            cout << "Quantos porcento gostaria de gerar de ruído? \n";
            cin >> porcentagem;
            gera_ruido(img_in, img_out, porcentagem);
            salva_img(Salvar, img_out);
        }
        else if (menu == 7)
        {
            cout << "Escreva o nome de como quer salvar a imagem\n";
            cin.getline(Salvar, 51);
            salva_img(Salvar, img_in);
            remove_ruido_mediana(img_in, img_out);
            salva_img(Salvar, img_out);
        }

        cout << "Deseja realizar mais alguma operação? (S/N) \n";
        cin >> respUsuario;
        if(respUsuario == 'N'){
            exit(0);
        }

        cout << "Utilizar a mesma imagem?  (S/N)\n";
        cin >> newImage;
    }

    return 0;
}

void abre_imagem(char nome[], Imagem &img_in)
{
    ifstream arquivo;
    arquivo.open(nome);

    if (arquivo.is_open())
    {
        arquivo >> img_in.type >> img_in.nc >> img_in.nl >> img_in.max;

        for (int i = 0; i < (img_in.nl); i++)
        {
            for (int j = 0; j < (img_in.nc); j++)
            {
                arquivo >> img_in.mat[i][j].r;
                arquivo >> img_in.mat[i][j].g;
                arquivo >> img_in.mat[i][j].b;
            }
        }

        char type[] = "P3";
        
        if (strcmp(type, img_in.type) != 0)
        {
            cout << "Arquivo de formato invalido\n";
            exit(0);
        }

        arquivo.close();
    }
    else
    {
        cout << "Erro em abrir imagem" << endl;
        exit(0);
    }
}

void salva_img(char Salvar[], Imagem img_in)
{
    ofstream img_out;
    img_out.open(Salvar);

    img_out << img_in.type << "\n"
            << img_in.nc << " " << img_in.nl << "\n"
            << img_in.max << "\n";

    for (int i = 0; i < (img_in.nl); i++)
    {
        for (int j = 0; j < (img_in.nc); j++)
        {
            img_out << img_in.mat[i][j].r << " ";
            img_out << img_in.mat[i][j].g << " ";
            img_out << img_in.mat[i][j].b << " ";
        }
        img_out << "\n";
    }

    img_out.close();
}

void solariza(Imagem img_in, Imagem &img_out, int limiar)
{
    stpcpy(img_out.type, img_in.type);
    img_out.nc = img_in.nc;
    img_out.nl = img_in.nl;
    img_out.max = img_in.max;

    for (int i = 0; i < (img_in.nl); i++)
    {
        for (int j = 0; j < (img_in.nc); j++)
        {
            if (img_in.mat[i][j].r < limiar)
            {
                img_out.mat[i][j].r = 255 - img_in.mat[i][j].r;
            }
            else
            {
                img_out.mat[i][j].r = img_in.mat[i][j].r;
            }

            if (img_in.mat[i][j].g < limiar)
            {
                img_out.mat[i][j].g = 255 - img_in.mat[i][j].g;
            }
            else
            {
                img_out.mat[i][j].g = img_in.mat[i][j].g;
            }

            if (img_in.mat[i][j].b < limiar)
            {
                img_out.mat[i][j].b = 255 - img_in.mat[i][j].b;
            }
            else
            {
                img_out.mat[i][j].b = img_in.mat[i][j].b;
            }
        }
    }
}

void rotaciona_horario(Imagem img_in, Imagem &img_out)
{
    strcpy(img_out.type, img_in.type);
    img_out.nc = img_in.nc;
    img_out.nl = img_in.nl;
    img_out.max = img_in.max;

    for (int i = 0; i < img_in.nl; i++)
    {
        for (int j = 0; j < img_in.nc; j++)
        {
            img_out.mat[i][j].r = img_in.mat[img_in.nc - 1 - j][i].r;
            img_out.mat[i][j].g = img_in.mat[img_in.nc - 1 - j][i].g;
            img_out.mat[i][j].b = img_in.mat[img_in.nc - 1 - j][i].b;
        }
    }
}

void rotaciona_antihorario(Imagem img_in, Imagem &img_out)
{
    strcpy(img_out.type, img_in.type);
    img_out.nc = img_in.nc;
    img_out.nl = img_in.nl;
    img_out.max = img_in.max;

    for (int i = 0; i < img_in.nc; i++)
    {
        for (int j = 0; j < img_in.nl; j++)
        {
            img_out.mat[i][j].r = img_in.mat[j][img_in.nl - 1 - i].r;
            img_out.mat[i][j].g = img_in.mat[j][img_in.nl - 1 - i].g;
            img_out.mat[i][j].b = img_in.mat[j][img_in.nl - 1 - i].b;
        }
    }
}

void aumenta_tamanho_vmp(Imagem img_in, Imagem &img_out)
{
    strcpy(img_out.type, img_in.type);
    img_out.nc = (2 * img_in.nc) - 1;
    img_out.nl = (2 * img_in.nl) - 1;
    img_out.max = img_in.max;

    for (int i = 0, k = 0; i < img_in.nl; i++, k += 2)
    {
        for (int j = 0, p = 0; j < img_in.nc; j++, p += 2)
        {
            img_out.mat[k][p].r = img_in.mat[i][j].r;
            img_out.mat[k][p].g = img_in.mat[i][j].g;
            img_out.mat[k][p].b = img_in.mat[i][j].b;

            img_out.mat[k][p + 1].r = img_in.mat[i][j].r;
            img_out.mat[k][p + 1].g = img_in.mat[i][j].g;
            img_out.mat[k][p + 1].b = img_in.mat[i][j].b;

            img_out.mat[k + 1][p].r = img_in.mat[i][j].r;
            img_out.mat[k + 1][p].g = img_in.mat[i][j].g;
            img_out.mat[k + 1][p].b = img_in.mat[i][j].b;

            img_out.mat[k + 1][p + 1].r = img_in.mat[i][j].r;
            img_out.mat[k + 1][p + 1].g = img_in.mat[i][j].g;
            img_out.mat[k + 1][p + 1].b = img_in.mat[i][j].b;

            if (i == img_in.nl)
            {
                k = img_out.nl;
                p = img_out.nc;
            }
        }
    }
}

void gera_ruido(Imagem img_in, Imagem &img_out, int porcentagem)
{
    strcpy(img_out.type, img_in.type);
    img_out.nc = img_in.nc;
    img_out.nl = img_in.nl;
    img_out.max = img_in.max;

    for (int i = 0; i < img_in.nl; i++)
    {
        for (int j = 0; j < img_in.nc; j++)
        {
            img_out.mat[i][j].r = img_in.mat[i][j].r;
            img_out.mat[i][j].g = img_in.mat[i][j].g;
            img_out.mat[i][j].b = img_in.mat[i][j].b;
        }
    }

    int cont = 0, i = 0, j = 0;
    while (cont < ((img_in.nl * img_out.nc * porcentagem) / 100))
    {
        i = (rand() % img_in.nl);
        j = (rand() % img_in.nc);

        int canal = (rand() % 3);

        if (canal == 0)
        {
            img_out.mat[i][j].r = 255;
            img_out.mat[i][j].g = 0;
            img_out.mat[i][j].b = 0;
        }
        else if (canal == 1)
        {
            img_out.mat[i][j].r = 0;
            img_out.mat[i][j].g = 255;
            img_out.mat[i][j].b = 0;
        }
        if (canal == 2)
        {
            img_out.mat[i][j].r = 0;
            img_out.mat[i][j].g = 0;
            img_out.mat[i][j].b = 255;
        }

        cont++;
    }
}

void preencher_vetor(int vetor_aux_red[], int vetor_aux_green[], int vetor_aux_blue[], Imagem &img_in, int i, int j)
{

    vetor_aux_red[0] = img_in.mat[i][j].r;
    vetor_aux_red[1] = img_in.mat[i + 1][j].r;
    vetor_aux_red[2] = img_in.mat[i - 1][j].r;
    vetor_aux_red[3] = img_in.mat[i][j - 1].r;
    vetor_aux_red[4] = img_in.mat[i][j + 1].r;
    vetor_aux_red[5] = img_in.mat[i + 1][j + 1].r;
    vetor_aux_red[6] = img_in.mat[i - 1][j + 1].r;
    vetor_aux_red[7] = img_in.mat[i + 1][j - 1].r;
    vetor_aux_red[8] = img_in.mat[i - 1][j - 1].r;

    vetor_aux_green[0] = img_in.mat[i][j].g;
    vetor_aux_green[1] = img_in.mat[i + 1][j].g;
    vetor_aux_green[2] = img_in.mat[i - 1][j].g;
    vetor_aux_green[3] = img_in.mat[i][j - 1].g;
    vetor_aux_green[4] = img_in.mat[i][j + 1].g;
    vetor_aux_green[5] = img_in.mat[i + 1][j + 1].g;
    vetor_aux_green[6] = img_in.mat[i - 1][j + 1].g;
    vetor_aux_green[7] = img_in.mat[i + 1][j - 1].g;
    vetor_aux_green[8] = img_in.mat[i - 1][j - 1].g;

    vetor_aux_blue[0] = img_in.mat[i][j].b;
    vetor_aux_blue[1] = img_in.mat[i + 1][j].b;
    vetor_aux_blue[2] = img_in.mat[i - 1][j].b;
    vetor_aux_blue[3] = img_in.mat[i][j - 1].b;
    vetor_aux_blue[4] = img_in.mat[i][j + 1].b;
    vetor_aux_blue[5] = img_in.mat[i + 1][j + 1].b;
    vetor_aux_blue[6] = img_in.mat[i - 1][j + 1].b;
    vetor_aux_blue[7] = img_in.mat[i + 1][j - 1].b;
    vetor_aux_blue[8] = img_in.mat[i - 1][j - 1].b;
}
void ordenar(int vetor[], int Tam)
{
    int aux;
    for (int fim = Tam; fim >= 0; fim--)
        for (int i = 0; i <= fim; i++)
        {
            if (vetor[i] > vetor[i + 1])
            {
                aux = vetor[i];
                vetor[i] = vetor[i + 1];
                vetor[i + 1] = aux;
            }
        }
}
void remove_ruido_mediana(Imagem img_in, Imagem &img_out)
{

    int vetor_aux_red[9];
    int vetor_aux_green[9];
    int vetor_aux_blue[9];

    strcpy(img_out.type, img_in.type);
    img_out.nc = img_in.nc;
    img_out.nl = img_in.nl;
    img_out.max = img_in.max;

    for (int i = 1; i < img_in.nl - 1; i++)
    {
        for (int j = 1; j < img_in.nc - 1; j++)
        {

            preencher_vetor(vetor_aux_red, vetor_aux_green, vetor_aux_blue, img_in, i, j);
            ordenar(vetor_aux_red, 9);
            ordenar(vetor_aux_green, 9);
            ordenar(vetor_aux_blue, 9);
            img_out.mat[i][j].r = vetor_aux_red[5];
            img_out.mat[i][j].g = vetor_aux_green[5];
            img_out.mat[i][j].b = vetor_aux_blue[5];
        }
    }
}
// ulimit -s 16384    -- comando para "segmentation fault"

