#include <stdlib.h>
#include <gtk/gtk.h>

#define FILENAME "resistores.txt"

struct resistores
{
    float resistencia;
    int serie;
    float tolerancia;
    float potencia;
    int quantidade;
};
typedef struct resistores Resistores;

struct serie
{
    struct serie *esquerda;
    int numero_serie;
    float valor_tolerancia;
    struct potencia* potencia;
    struct serie *direita;
};
typedef struct serie Serie;

struct potencia
{
    struct potencia *esquerda;
    float potencia_resistor;
    struct resistencia* resistencia;
    struct potencia *direita;
};
typedef struct potencia Potencia;

struct resistencia
{
    struct resistencia *esquerda;
    float valor_resistencia;
    int quantidade;
    struct resistencia *direita;
};
typedef struct resistencia Resistencia;

struct lista
{
    struct lista* listaanterior;
    struct resistores* ponteirodedados;
    struct lista* listaprox;
};
typedef struct lista Lista;


void insere_resistor(Resistencia**Arvore, float valor_resistencia ,int quantidade)
{
    if(*Arvore == NULL)
    {
        Resistencia* ResistenciaN = (Resistencia*) malloc(sizeof(Resistencia));
        ResistenciaN->esquerda = NULL;
        ResistenciaN->direita = NULL;
        ResistenciaN->valor_resistencia = valor_resistencia;
        ResistenciaN->quantidade=quantidade;
        *Arvore=ResistenciaN;
    }
    else
    {
        if(valor_resistencia < (*Arvore)->valor_resistencia)
            insere_resistor(&(*Arvore)->esquerda,valor_resistencia,quantidade);

        if(valor_resistencia > (*Arvore)->valor_resistencia)
            insere_resistor(&(*Arvore)->direita, valor_resistencia,quantidade);

        if(valor_resistencia == (*Arvore)->valor_resistencia)
            (*Arvore)->quantidade+=quantidade;
    }
}

void inserir_potencia(Potencia**Arvore, float potencia_resistor, float valor_resistencia ,int quantidade)
{
    if(*Arvore == NULL)
    {
        Potencia* PotenciaN = (Potencia*) malloc(sizeof(Potencia));
        PotenciaN->esquerda = NULL;
        PotenciaN->direita = NULL;
        PotenciaN->resistencia =NULL;
        PotenciaN->potencia_resistor = potencia_resistor;
        *Arvore=PotenciaN;
        insere_resistor(&PotenciaN->resistencia,valor_resistencia,quantidade);
    }
    else
    {
        if(potencia_resistor < (*Arvore)->potencia_resistor)
            inserir_potencia(&(*Arvore)->esquerda,potencia_resistor,valor_resistencia,quantidade);

        if(potencia_resistor > (*Arvore)->potencia_resistor)
            inserir_potencia(&(*Arvore)->direita,potencia_resistor,valor_resistencia,quantidade);

        if(potencia_resistor == (*Arvore)->potencia_resistor)
            insere_resistor(&(*Arvore)->resistencia,valor_resistencia,quantidade);
    }
}

void inserir(Serie**Arvore, int numero_serie,float potencia_resistor,float valor_resistencia ,int quantidade)
{
    if(*Arvore == NULL)
    {
        Serie* SerieN = (Serie*) malloc(sizeof(Serie));
        SerieN->esquerda = NULL;
        SerieN->direita = NULL;
        SerieN->numero_serie = numero_serie;
        //SerieN->valor_tolerancia=valor_tolerancia;
        SerieN->potencia = NULL;
        *Arvore=SerieN;
        inserir_potencia(&(*Arvore)->potencia,potencia_resistor,valor_resistencia,quantidade);
    }
    else
    {
        if(numero_serie < (*Arvore)->numero_serie)
            inserir(&(*Arvore)->esquerda,numero_serie,potencia_resistor,valor_resistencia,quantidade);

        if(numero_serie > (*Arvore)->numero_serie)
            inserir(&(*Arvore)->direita, numero_serie,potencia_resistor,valor_resistencia,quantidade);

        if(numero_serie == (*Arvore)->numero_serie)
            inserir_potencia(&(*Arvore)->potencia,potencia_resistor,valor_resistencia,quantidade);
    }
}



void mostra_ordem_res(Resistencia**Arvore,int numero_serie,float potencia_resistor,FILE * fp)
{
    if((*Arvore) != NULL)
    {
        mostra_ordem_res(&(*Arvore)->esquerda,numero_serie,potencia_resistor,fp);
        printf("\n   Série: E%d\tPotência:%5.2f\tResistencia:%9.1f\tQuantidade:%6.d",numero_serie, potencia_resistor, (*Arvore)->valor_resistencia, (*Arvore)->quantidade);
        fprintf(fp,"%d\t %5.2f %12.1f %6.d\n",numero_serie, potencia_resistor, (*Arvore)->valor_resistencia, (*Arvore)->quantidade);
        mostra_ordem_res(&(*Arvore)->direita,numero_serie,potencia_resistor,fp);
    }
}

void mostrar_ordem_pot(Potencia**Arvore, int numero_serie,FILE * fp)
{
    if((*Arvore) != NULL)
    {
        mostrar_ordem_pot(&(*Arvore)->esquerda,numero_serie,fp);
        mostra_ordem_res(&(*Arvore)->resistencia,numero_serie,(*Arvore)->potencia_resistor,fp);
        mostrar_ordem_pot(&(*Arvore)->direita,numero_serie,fp);
    }
}

void mostra_tudo_ordem(Serie**Arvore)
{
    FILE *fs;
    fs = fopen(FILENAME,"w+");
    if((*Arvore) != NULL)
    {
        mostra_tudo_ordem(&(*Arvore)->esquerda);
        mostrar_ordem_pot(&(*Arvore)->potencia,(*Arvore)->numero_serie,fs);
        mostra_tudo_ordem(&(*Arvore)->direita);
    }
    fclose(fs);
}

void busca_resitencia(Resistencia**Arvore,float valor_resistencia ,int quantidade)
{
    if((*Arvore) != NULL)
    {
        busca_resitencia(&(*Arvore)->esquerda,valor_resistencia,quantidade);
        if(valor_resistencia == (*Arvore)->valor_resistencia  || valor_resistencia ==0 || (valor_resistencia==0 &&quantidade==(*Arvore)->quantidade))
            printf("\n      Resistencia:%8.1f  Quantidade:%4.d", (*Arvore)->valor_resistencia, (*Arvore)->quantidade);

        busca_resitencia(&(*Arvore)->direita,valor_resistencia,quantidade);
    }
}

void busca_potencia(Potencia**Arvore, float potencia_resistor, float valor_resistencia ,int quantidade)
{
    if((*Arvore) != NULL)
    {
        busca_potencia(&(*Arvore)->esquerda,potencia_resistor,valor_resistencia,quantidade);
        if(potencia_resistor == (*Arvore)->potencia_resistor || potencia_resistor ==0)
        {
            printf("\n  Potência:%6.2f", (*Arvore)->potencia_resistor);
            busca_resitencia(&(*Arvore)->resistencia,valor_resistencia,quantidade);
        }

        //mostrar_ordem_pot(&(*Arvore)->potencia);
        busca_potencia(&(*Arvore)->direita,potencia_resistor,valor_resistencia,quantidade);
    }
}

void busca_serie(Serie**Arvore, int numero_serie,float potencia_resistor,float valor_resistencia ,int quantidade)
{
    if((*Arvore) != NULL)
    {
        busca_serie(&(*Arvore)->esquerda,numero_serie,potencia_resistor,valor_resistencia,quantidade);
        if(numero_serie == (*Arvore)->numero_serie || numero_serie ==0)
        {
            printf("\nSérie:E%d", (*Arvore)->numero_serie);
            busca_potencia(&(*Arvore)->potencia,potencia_resistor,valor_resistencia,quantidade);
        }
        busca_serie(&(*Arvore)->direita,numero_serie,potencia_resistor,valor_resistencia,quantidade);
    }
}


void adiciona_res(Serie**Arvore)
{
    int serie=0,quantidade=0,i=0;
    float resitencia=0,potencia=0,tolerancia=0;
    char resposta;
    printf("\n Entre com os valores de: \n\n Resistência:");
    scanf("%f",&resitencia);
    printf(" Série:");
    scanf("%d",&serie);
    printf(" Tolerância:");
    scanf("%f",&tolerancia);
    printf(" Potência:");
    scanf("%f",&potencia);
    printf(" Quantidade:");
    scanf("%d",&quantidade);
    fflush(stdin);
    printf("\n Os Valores estão corretos? (S/N) \n Resis.= %8.1f  Serie.= E%.2d Toler. = %5.2f Poten.= %5.2f  Quant.= %.4d  \n",resitencia,serie,tolerancia,potencia,quantidade);
    scanf("%c",&resposta);
    if (resposta=='S' || resposta=='s')
    {
        if (serie==0 || serie==6 || serie==12 || serie==24 || serie==48 || serie==96 || serie==192)
        {
            if(serie==0 && tolerancia==0)
            {
                printf(" Informe pelo menos um dos itens abaixo:\n Série \n Tolerância \n");
            }

            if(serie==192 && (!( tolerancia== 0.50 || tolerancia== 0.25 || tolerancia== 0.10 )))
            {
                if(serie==0 && tolerancia==0)
            {
                printf(" Informe pelo menos um dos itens abaixo:\n Série \n Tolerância \n");
                i++;
            }
                printf(" Para resistores da série E192 é obrigatório informar a tolerância \n");
                while(1)
                {
                    fflush(stdin);
                    printf(" Tolerância:");
                    scanf("%f",&tolerancia);

                    if (tolerancia==0.5 || tolerancia==0.25 || tolerancia==0.10)
                        break;
                    else
                        printf(" A tolerância informada é inválida, tente novamente\n");
                }
            }

            if (serie==24)
                tolerancia=5;
            else if (serie==12)
                tolerancia=10;
            else if (serie==48)
                tolerancia=2;
            else if (serie==6)
                tolerancia=20;
            else if (serie==96)
                tolerancia=1;
            if (tolerancia==20)
                serie=6;
            else if (tolerancia==10)
                serie=12;
            else if (tolerancia==5)
                serie=24;
            else if (tolerancia==2)
                serie=48;
            else if (tolerancia==1)
                serie=96;

            if(i==0)
            inserir(&(*Arvore),serie,potencia,resitencia,quantidade);

        }
        else
            printf(" Esta série não existe\n Este resistor não pode ser adicionado\n");
    }
    else
        printf(" A adição do resistor foi cancelada\n");
}

/*static void helloWorld (GtkWidget *wid, GtkWidget *win)
{
  GtkWidget *dialog = NULL;

  dialog = gtk_message_dialog_new (GTK_WINDOW (win), GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_CLOSE, "Hello World!");
  gtk_window_set_position (GTK_WINDOW (dialog), GTK_WIN_POS_CENTER);
  gtk_dialog_run (GTK_DIALOG (dialog));
  gtk_widget_destroy (dialog);
}*/

void button_clicked_mostra_tudo(GtkWidget *widget, gpointer data)
{
    int n;
    int serie=0,quantidade=0;
    float resitencia=0,potencia=0;
    FILE *fp;
    Serie*Arvore;
    Arvore = NULL;
    fp = fopen(FILENAME,"r");
    if (fp == NULL)
    {
        show_error();
        //printf("Erro ao abrir arquivo %s.\n", FILENAME);
        exit(1);
    }
    while(1)
    {
        n=fscanf(fp,"%d %f %f %d",&serie,&potencia,&resitencia,&quantidade);
        if (n==EOF)
            break;
        inserir(&Arvore,serie,potencia,resitencia,quantidade);
    }
    mostra_tudo_ordem(&Arvore);
    g_print("clicked\n");
}


void show_error(GtkWidget *widget, gpointer window) {

  GtkWidget *dialog;
  dialog = gtk_message_dialog_new(GTK_WINDOW(window),
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_OK,
            "Error loading file");
  gtk_window_set_title(GTK_WINDOW(dialog), "Error");
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}

int main (int argc, char *argv[])
{
    int n;
    int serie=0,quantidade=0;
    float resitencia=0,potencia=0;
    FILE *fp;
    Serie*Arvore;
    Arvore = NULL;
    fp = fopen(FILENAME,"r");
    if (fp == NULL)
    {
        printf("Erro ao abrir arquivo %s.\n", FILENAME);
        exit(1);
    }
    while(1)
    {
        n=fscanf(fp,"%d %f %f %d",&serie,&potencia,&resitencia,&quantidade);
        if (n==EOF)
            break;
        inserir(&Arvore,serie,potencia,resitencia,quantidade);
    }
    GtkWidget *window;
    GtkWidget *vbox;

    GtkWidget *menubar;
    GtkWidget *fileMenu;
    GtkWidget *fileMi;
    GtkWidget *mostra_tudoMi;
    GtkWidget *quitMi;
    GtkWidget *pesquisaMi;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
    gtk_window_set_title(GTK_WINDOW(window), "Simple menu");

    vbox = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    menubar = gtk_menu_bar_new();
    fileMenu = gtk_menu_new();

    fileMi = gtk_menu_item_new_with_label("Arquivo");
    mostra_tudoMi = gtk_menu_item_new_with_label("Mostrar Estoque");
    pesquisaMi = gtk_menu_item_new_with_label("Pesquisar Estoque");
    quitMi = gtk_menu_item_new_with_label("Sair");

    gtk_menu_item_set_submenu(GTK_MENU_ITEM(fileMi), fileMenu);
    gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), mostra_tudoMi);
    gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), pesquisaMi);
    gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), quitMi);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), fileMi);
    gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, FALSE, 0);

        g_signal_connect(G_OBJECT(window), "destroy",
            G_CALLBACK(gtk_main_quit), NULL);

    g_signal_connect(G_OBJECT(quitMi), "activate",
            G_CALLBACK(gtk_main_quit), NULL);

    g_signal_connect(G_OBJECT(mostra_tudoMi), "activate",
            G_CALLBACK(button_clicked_mostra_tudo), NULL);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
