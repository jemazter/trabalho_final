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
        //printf("ok - 1");
        mostra_ordem_res(&(*Arvore)->esquerda,numero_serie,potencia_resistor,fp);
        printf("\n E%d\t%5.2f\t\t%9.1f\t%6.d",numero_serie, potencia_resistor, (*Arvore)->valor_resistencia, (*Arvore)->quantidade);
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

void mostra_tudo_ordem(Serie**Arvore, FILE *fp)
{

    if((*Arvore) != NULL)
    {
        mostra_tudo_ordem(&(*Arvore)->esquerda,fp);
        mostrar_ordem_pot(&(*Arvore)->potencia,(*Arvore)->numero_serie,fp);
        mostra_tudo_ordem(&(*Arvore)->direita,fp);
    }

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
    int serie=0,quantidade=0;
    float resitencia=0,potencia=0;
    char op;
    printf("\n Entre com os valores de: \n\n Resistência:");
    scanf("%f",&resitencia);
    printf(" Série:");
    scanf("%d",&serie);
    printf(" Potência:");
    scanf("%f",&potencia);
    printf(" Quantidade:");
    scanf("%d",&quantidade);
    fflush(stdin);
    printf("\n Confirmar os valores? (s/n) \n Resis.= %8.1f  Serie.= E%.2d Poten.= %5.2f  Quant.= %.4d  \n",resitencia,serie,potencia,quantidade);
    scanf("%c",&op);
    if (op=='S' || op=='s')
            inserir(&(*Arvore),serie,potencia,resitencia,quantidade);
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
    system("cls");
    int n;
    int serie=0,quantidade=0;
    float resitencia=0,potencia=0;
    FILE *fp;
    Serie*Arvore;
    Arvore = NULL;
    fp = fopen(FILENAME,"r");
    if (fp == NULL)
    {
        //printf("ok");
        printf("Erro ao abrir arquivo %s.\n", FILENAME);
        exit(1);
    }
    //printf("ok1");
    while(1)
    {
        n=fscanf(fp,"%d %f %f %d",&serie,&potencia,&resitencia,&quantidade);
        if (n==EOF)
            break;
        inserir(&Arvore,serie,potencia,resitencia,quantidade);
    }
    FILE *fs;
    fs = fopen(FILENAME,"w+");
    printf("\n__________________________________________________\n");
    printf("\nSérie\tPotência\tResistencia\tQuantidade");
    printf("\n__________________________________________________\n");
    mostra_tudo_ordem(&Arvore, fs);
    fclose(fs);
    //g_print("clicked\n");
}

void button_clicked_adiciona(GtkWidget *widget, gpointer data)
{
    system("cls");
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

    adiciona_res(&Arvore);
    FILE *fs;
    fs = fopen(FILENAME,"w+");
    printf("\n__________________________________________________\n");
    printf("\nSérie\tPotência\tResistencia\tQuantidade");
    printf("\n__________________________________________________\n");
    mostra_tudo_ordem(&Arvore, fs);
    fclose(fs);
}

void button_clicked_pesquisa(GtkWidget *widget, gpointer data)
{
    system("cls");
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
    printf("SISTEMA DE BUSCA\nDIGITE 0 (ZERO) NOS CAMPOS EM QUE NAO DESEJA ESPECIFICAR VALOR.");
    printf("\n Entre com o valor de busca (resistencia):");
    scanf("%f",&resitencia);
    printf("\n Entre com o valor de busca (serie):");
    scanf("%d",&serie);
    printf("\n Entre com o valor de busca (potencia):");
    scanf("%f",&potencia);
    printf("\n Entre com o valor de busca (quantidade):");
    scanf("%d",&quantidade);
    busca_serie(&Arvore,serie,potencia,resitencia,quantidade);
}

void show_info(GtkWidget *widget, gpointer window) {

  GtkWidget *dialog;
  dialog = gtk_message_dialog_new(GTK_WINDOW(window),
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_INFO,
            GTK_BUTTONS_OK,
            "Esta é a interface grafica do programa decontrole de resistores./nOs dados serão exibidos na janela de console.");
  gtk_window_set_title(GTK_WINDOW(dialog), "Atenção!");
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}

int main (int argc, char *argv[])
{
    int i=0;
    GtkWidget *window;
    GtkWidget *vbox;

    GtkWidget *menubar;
    GtkWidget *fileMenu;
    GtkWidget *fileMi;
    GtkWidget *mostra_tudoMi;
    GtkWidget *quitMi;
    GtkWidget *pesquisaMi;
    GtkWidget *adicionaMi;
    //GtkWidget *label;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window), 200, 200);
    gtk_window_set_title(GTK_WINDOW(window), "Catalogo de Resistores");

    vbox = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    menubar = gtk_menu_bar_new();
    fileMenu = gtk_menu_new();



    fileMi = gtk_menu_item_new_with_label("Arquivo");
    mostra_tudoMi = gtk_menu_item_new_with_label("Mostrar Estoque");
    pesquisaMi = gtk_menu_item_new_with_label("Pesquisar Estoque");
    quitMi = gtk_menu_item_new_with_label("Sair");
    adicionaMi=gtk_menu_item_new_with_label("Adicionar Resistor");

    gtk_menu_item_set_submenu(GTK_MENU_ITEM(fileMi), fileMenu);
    gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), mostra_tudoMi);
    gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), pesquisaMi);
    gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), adicionaMi);
    gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), quitMi);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), fileMi);
    gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, FALSE, 0);

    g_signal_connect(G_OBJECT(window), "destroy",
            G_CALLBACK(gtk_main_quit), NULL);

    g_signal_connect(G_OBJECT(quitMi), "activate",
            G_CALLBACK(gtk_main_quit), NULL);

    g_signal_connect(G_OBJECT(quitMi), "activate",
            G_CALLBACK(gtk_main_quit), NULL);

    g_signal_connect(G_OBJECT(mostra_tudoMi), "activate",
            G_CALLBACK(button_clicked_mostra_tudo), NULL);

    g_signal_connect(G_OBJECT(adicionaMi), "activate",
            G_CALLBACK(button_clicked_adiciona), NULL);

    g_signal_connect(G_OBJECT(pesquisaMi), "activate",
            G_CALLBACK(button_clicked_pesquisa), NULL);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
