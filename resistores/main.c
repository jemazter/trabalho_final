#include <stdlib.h>
#include <gtk/gtk.h>


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
    int quantidade_resistores;
    struct resistencia *direita;
};
typedef struct resistencia Resistencia;

struct header
{
    struct lista* iniciodeheader;
    int nelementos;
    struct lista* fimdeheader;
};
typedef struct header Estruturadoheader;

struct lista
{
    struct lista* listaanterior;
    struct resistores* ponteirodedados;
    struct lista* listaprox;
};
typedef struct lista Lista;

void inic_header(Estruturadoheader* Header)
{
    Header->nelementos = 0;  //inicializando
    Header->iniciodeheader = NULL;   //inicializando
    Header->fimdeheader = NULL;      //inicializando
}

void insere_resistor(Resistencia**Arvore, float valor_resistencia ,int quantidade_resistores)
{
    if(*Arvore == NULL)
    {
        Resistencia* ResistenciaN = (Resistencia*) malloc(sizeof(Resistencia));
        ResistenciaN->esquerda = NULL;
        ResistenciaN->direita = NULL;
        ResistenciaN->valor_resistencia = valor_resistencia;
        ResistenciaN->quantidade_resistores=quantidade_resistores;
        *Arvore=ResistenciaN;
    }
    else
    {
        if(valor_resistencia < (*Arvore)->valor_resistencia)
            insere_resistor(&(*Arvore)->esquerda,valor_resistencia,quantidade_resistores);

        if(valor_resistencia > (*Arvore)->valor_resistencia)
            insere_resistor(&(*Arvore)->direita, valor_resistencia,quantidade_resistores);

        if(valor_resistencia == (*Arvore)->valor_resistencia)
            (*Arvore)->quantidade_resistores+=quantidade_resistores;
    }
}
/*static void helloWorld (GtkWidget *wid, GtkWidget *win)
{
  GtkWidget *dialog = NULL;

  dialog = gtk_message_dialog_new (GTK_WINDOW (win), GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_CLOSE, "Hello World!");
  gtk_window_set_position (GTK_WINDOW (dialog), GTK_WIN_POS_CENTER);
  gtk_dialog_run (GTK_DIALOG (dialog));
  gtk_widget_destroy (dialog);
}*/

int main (int argc, char *argv[])
{
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

  gtk_widget_show_all(window);

  gtk_main();

  return 0;
}
