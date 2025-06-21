/*
 * Aluno: Thiago Souza Pereira
 * Matrícula: 202121148
 * Disciplina: Computação Gráfica
 * Docente: Vânia
 * Pokemón - Umbreon
 * Atividade 6 - Animação
 *
 * ----------------------------------------------------------------------
 * COMANDOS DO TECLADO:
 * 
 * 
 * Modos de animação automática:
 *   p ou P      - Alterna modo "Balada" 
 *   l ou L      - Alterna modo "Ladin" 
 *   c ou C      - Alterna modo "Desenrola" 
 * 
 * Controle manual das articulações (modo parado):
 * 
 *   q / Q       - Gira base da cauda para cima
 *   a / A       - Gira base da cauda para baixo
 *   w / W       - Gira meio da cauda para cima
 *   s / S       - Gira meio da cauda para baixo
 *   e / E       - Gira ponta da cauda para cima
 *   d / D       - Gira ponta da cauda para baixo
 *   r / R       - Sobe base da orelha esquerda
 *   f / F       - Desce base da orelha esquerda
 *   t / T       - Sobe ponta da orelha esquerda
 *   g / G       - Desce ponta da orelha esquerda
 *   y / Y       - Sobe base da orelha direita
 *   h / H       - Desce base da orelha direita
 *   u / U       - Sobe ponta da orelha direita
 *   j / J       - Desce ponta da orelha direita
 *   z / Z       - Gira cabeça para a esquerda
 *   x / X       - Gira cabeça para a direita
 * 
 * Tamanho do Umbreon:
 * 
 *   Page Up     - Aumenta o tamanho do Umbreon
 *   Page Down   - Diminui o tamanho do Umbreon
 *   m / M       - Aumenta o tamanho do Umbreon (alternativo ao Page Up)
 *   n / N       - Diminui o tamanho do Umbreon (alternativo ao Page Down)
 * 
 * Teclas Especiais:
 * 
 *   Seta Cima       - Move Umbreon para cima
 *   Seta Baixo      - Move Umbreon para baixo
 *   Seta Esquerda   - Move Umbreon para a esquerda
 *   Seta Direita    - Move Umbreon para a direita
 * 
 * Obs: Ao ativar um modo de animação automática, qualquer comando manual retorna para o modo dele parado.
 * 
 *  ----------------------------------------------------------------------
 * ----------------------------------------------------------------------
 */

#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// ----------------------
// Definições de cores e parâmetros visuais
// ----------------------
#define COR_CORPO_UMBREON 0.18f, 0.20f, 0.24f
#define COR_CONTORNO_CORPO_UMBREON 0.03f, 0.03f, 0.05f
#define COR_ACENTO_ESCURO_CORPO_UMBREON 0.14f, 0.16f, 0.19f
#define COR_ANEIS_AMARELOS 1.0f, 0.843f, 0.0f
#define COR_CONTORNO_ANEIS_AMARELOS 0.1f, 0.1f, 0.1f
#define COR_OLHO_VERMELHO 0.9f, 0.1f, 0.1f
#define COR_PUPILA_OLHO 0.05f, 0.05f, 0.05f
#define COR_BRILHO_OLHO 1.0f, 1.0f, 1.0f
#define COR_CONTORNO_OLHO 0.03f, 0.03f, 0.03f
#define COR_CEU_NOITE 0.05f, 0.05f, 0.15f
#define COR_CHAO_NOITE 0.12f, 0.20f, 0.12f
#define COR_LUA 0.9f, 0.9f, 0.8f

#ifndef PI
#define PI 3.14159265358979323846
#endif

#define NUM_SEGMENTOS_OVAL 60
#define LARGURA_LINHA_PADRAO 1.5f
#define LARGURA_CONTORNO_ANEL 1.0f
#define LIMITE_ANGULO_CABECA 45.0f
#define LIMITE_ANGULO_ORELHA_BASE 40.0f
#define LIMITE_ANGULO_ORELHA_PONTA 35.0f
#define ZONA_PROIBIDA_CAUDA_FIM   -45.0f
#define ZONA_PROIBIDA_CAUDA_INICIO -135.0f
#define LIMITE_CAUDA_MEIO 60.0f
#define LIMITE_CAUDA_PONTA 60.0f
#define LIMITE_ANGULO_BRACO 60.0f

// ----------------------
// Variáveis globais de estado e animação
// ----------------------
int largura_janela_global = 640;
int altura_janela_global = 640;

// Controle de posição e escala
GLfloat g_deslocamento_x = 0.0f;
GLfloat g_deslocamento_y = 0.0f;
GLfloat g_incremento_deslocamento = 10.0f;
GLfloat g_escala_umbreon = 1.0f;
GLfloat g_incremento_escala = 0.1f;
const GLfloat ESCALA_MINIMA = 0.3f;
const GLfloat ESCALA_MAXIMA = 2.8f;

// Articulações da cauda
GLfloat g_angulo_cauda_base = 0.0f;
GLfloat g_angulo_cauda_meio = 0.0f;
GLfloat g_angulo_cauda_ponta = 10.0f;
GLfloat g_incremento_angulo_cauda = 5.0f;

// Articulações das orelhas
GLfloat g_angulo_orelha_E_base = 0.0f;
GLfloat g_angulo_orelha_E_ponta = 0.0f;
GLfloat g_angulo_orelha_D_base = 0.0f;
GLfloat g_angulo_orelha_D_ponta = 0.0f;
GLfloat g_incremento_angulo_orelha = 5.0f;

// Cabeça e olhos
GLfloat g_angulo_cabeca = 0.0f;
GLfloat g_incremento_angulo_cabeca = 5.0f;
GLfloat g_fator_palpebra_y = 1.0f; // Para animar a piscada
int g_estado_piscada = 0;
int g_tempo_piscada_contador = 0;
const int DURACAO_FASE_PISCADA = 3;
const int TEMPO_OLHOS_FECHADOS = 4;
const int INTERVALO_ENTRE_PISCADAS = 180;

// Braços e corpo (para animação)
GLfloat g_angulo_braco_E = 0.0f;
GLfloat g_angulo_braco_D = 0.0f;
GLfloat g_angulo_corpo = 0.0f;

// Controle dos modos de animação
int g_modo_animacao = 0; // 0=Parado, 1=Balada, 2=Ladin, 3=Desenrola
GLfloat g_tempo_animacao = 0.0f;
GLfloat g_danca_offset_x = 0.0f;
GLfloat g_danca_offset_y = 0.0f;
int g_fase_coreografia = 0;
int g_tempo_fase = 0;

// Suavização das orelhas (para animação mais natural)
float g_orelha_E_base_alvo = 0.0f;
float g_orelha_D_base_alvo = 0.0f;
float g_orelha_E_ponta_alvo = 0.0f;
float g_orelha_D_ponta_alvo = 0.0f;
const float ORELHA_BASE_SUAVIZACAO = 0.18f;
const float ORELHA_PONTA_SUAVIZACAO = 0.09f;

// ----------------------
// Protótipos das funções
// ----------------------
void desenhar_oval_preenchida(float cx, float cy, float rx, float ry, int num_segmentos);
void desenhar_contorno_oval(float cx, float cy, float rx, float ry, int num_segmentos);
void desenhar_poligono_preenchido(float vertices[][2], int num_vertices);
void desenhar_contorno_poligono(float vertices[][2], int num_vertices);
void desenhar_anel_fita_avancado(float cx, float cy, float rx_externo, float ry_externo, float rx_interno, float ry_interno, int num_segmentos, const float cor_preenchimento[], const float cor_contorno[]);
void desenhar_cauda_umbreon_hierarquica(float pivo_x_corpo, float pivo_y_corpo);
void desenhar_segmento_simples(float largura_base, float largura_topo, float comprimento, const float cor_preench[], const float cor_contorno[]);
void desenhar_orelha_umbreon_hierarquica(float pivo_x_na_cabeca, float pivo_y_na_cabeca, float comprimento_total_orelha, int lado);
void desenhar_perna_umbreon(float pivo_x, float pivo_y, int lado, float ref_largura_topo_corpo, float altura_real_perna, float raio_externo_anel_perna, int is_arm);
void desenhar_cenario(void);
void exibir(void);
void reshape(int w, int h);
void gerenciar_teclado(unsigned char tecla, int x, int y);
void gerenciar_teclas_especiais(int tecla, int x, int y);
void animacao_loop(int valor);
void resetar_animacao();

// ----------------------
// Funções de desenho geométrico reutilizáveis
// ----------------------

// Desenha uma oval preenchida
void desenhar_oval_preenchida(float cx, float cy, float rx, float ry, int num_segmentos) {
    if (ry < 0.05f && ry > -0.05f && num_segmentos > 2) { return; }
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= num_segmentos; i++) {
        float angulo = i * 2.0f * PI / num_segmentos;
        glVertex2f(cx + (rx * cos(angulo)), cy + (ry * sin(angulo)));
    }
    glEnd();
}

// Desenha o contorno de uma oval
void desenhar_contorno_oval(float cx, float cy, float rx, float ry, int num_segmentos_param) {
    if (ry < 0.05f && ry > -0.05f) {
        glBegin(GL_LINES);
        glVertex2f(cx - rx, cy);
        glVertex2f(cx + rx, cy);
        glEnd();
        return;
    }
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i <= num_segmentos_param; i++) {
        float angulo = i * 2.0f * PI / num_segmentos_param;
        glVertex2f(cx + (rx * cos(angulo)), cy + (ry * sin(angulo)));
    }
    glEnd();
}

// Desenha um polígono preenchido
void desenhar_poligono_preenchido(float vertices[][2], int num_vertices) {
    glBegin(GL_POLYGON);
    for (int i = 0; i < num_vertices; i++) {
        glVertex2f(vertices[i][0], vertices[i][1]);
    }
    glEnd();
}

// Desenha o contorno de um polígono
void desenhar_contorno_poligono(float vertices[][2], int num_vertices) {
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < num_vertices; i++) {
        glVertex2f(vertices[i][0], vertices[i][1]);
    }
    glEnd();
}

// Desenha um anel (usado nas marcas amarelas)
void desenhar_anel_fita_avancado(float cx, float cy, float rx_externo, float ry_externo, float rx_interno, float ry_interno, int num_segmentos_param, const float cor_preenchimento[], const float cor_contorno[]) {
    glColor3fv(cor_preenchimento);
    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i <= num_segmentos_param; i++) {
        float angulo = i * 2.0f * PI / num_segmentos_param;
        float cos_a = cos(angulo);
        float sin_a = sin(angulo);
        glVertex2f(cx + rx_externo * cos_a, cy + ry_externo * sin_a);
        glVertex2f(cx + rx_interno * cos_a, cy + ry_interno * sin_a);
    }
    glEnd();
    if (cor_contorno != NULL) {
        glLineWidth(LARGURA_CONTORNO_ANEL);
        glColor3fv(cor_contorno);
        desenhar_contorno_oval(cx, cy, rx_externo, ry_externo, num_segmentos_param);
        desenhar_contorno_oval(cx, cy, rx_interno, ry_interno, num_segmentos_param);
        glLineWidth(LARGURA_LINHA_PADRAO);
    }
}

// Desenha um segmento simples (trapézio)
void desenhar_segmento_simples(float largura_base, float largura_topo, float comprimento, const float cor_preench[], const float cor_contorno[]) {
    float vertices_segmento[][2] = {
        {-largura_base / 2.0f, 0.0f},
        { largura_base / 2.0f, 0.0f},
        { largura_topo / 2.0f, comprimento},
        {-largura_topo / 2.0f, comprimento}
    };
    glColor3fv(cor_preench);
    desenhar_poligono_preenchido(vertices_segmento, 4);
    glColor3fv(cor_contorno);
    desenhar_contorno_poligono(vertices_segmento, 4);
}

// Desenha a cauda do Umbreon (hierarquia de 3 segmentos)
void desenhar_cauda_umbreon_hierarquica(float pivo_x_corpo, float pivo_y_corpo) {
    const float cor_corp[] = {COR_CORPO_UMBREON};
    const float cor_cont_corp[] = {COR_CONTORNO_CORPO_UMBREON};
    glPushMatrix();
    glTranslatef(pivo_x_corpo, pivo_y_corpo, 0.0f);
    glPushMatrix();
    glRotatef(g_angulo_cauda_base, 0.0f, 0.0f, 1.0f);
    desenhar_segmento_simples(12.0f, 10.0f, 35.0f, cor_corp, cor_cont_corp);
    glTranslatef(0.0f, 35.0f, 0.0f);
    glPushMatrix();
    glRotatef(g_angulo_cauda_meio, 0.0f, 0.0f, 1.0f);
    desenhar_segmento_simples(10.0f, 8.0f, 30.0f, cor_corp, cor_cont_corp);
    glTranslatef(0.0f, 30.0f, 0.0f);
    glPushMatrix();
    glRotatef(g_angulo_cauda_ponta, 0.0f, 0.0f, 1.0f);
    desenhar_segmento_simples(8.0f, 5.0f, 25.0f, cor_corp, cor_cont_corp);
    // Anel amarelo na ponta da cauda
    const float cor_anel[] = {COR_ANEIS_AMARELOS};
    const float cor_cont_anel[] = {COR_CONTORNO_ANEIS_AMARELOS};
    desenhar_anel_fita_avancado(0.0f, 25.0f, 5.0f * 1.2f, 7.0f, 5.0f * 1.2f * 0.65f, 7.0f * 0.65f, NUM_SEGMENTOS_OVAL, cor_anel, cor_cont_anel);
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
}

// Desenha uma orelha (hierarquia de 2 segmentos)
void desenhar_orelha_umbreon_hierarquica(float pivo_x_na_cabeca, float pivo_y_na_cabeca, float comprimento_total_orelha, int lado) {
    float comp_seg_base_orelha = comprimento_total_orelha * 0.6f;
    float larg_base_seg_base_orelha = 15.0f;
    float larg_topo_seg_base_orelha = 12.0f;
    float comp_seg_ponta_orelha = comprimento_total_orelha * 0.4f;
    float larg_base_seg_ponta_orelha = larg_topo_seg_base_orelha;
    float larg_topo_seg_ponta_orelha = 6.0f;
    GLfloat angulo_base_atual = (lado == -1) ? g_angulo_orelha_E_base : g_angulo_orelha_D_base;
    GLfloat angulo_ponta_atual = (lado == -1) ? g_angulo_orelha_E_ponta : g_angulo_orelha_D_ponta;
    const float cor_orelha[] = {COR_ACENTO_ESCURO_CORPO_UMBREON};
    const float cor_contorno_orelha[] = {COR_CONTORNO_CORPO_UMBREON};
    glPushMatrix();
    glTranslatef(pivo_x_na_cabeca * lado, pivo_y_na_cabeca, 0.0f);
    glRotatef(lado * 20.0f, 0.0f, 0.0f, 1.0f);
    glPushMatrix();
    glRotatef(angulo_base_atual, 0.0f, 0.0f, 1.0f);
    desenhar_segmento_simples(larg_base_seg_base_orelha, larg_topo_seg_base_orelha, comp_seg_base_orelha, cor_orelha, cor_contorno_orelha);

    // Anel amarelo na orelha
    const float cor_preench_anel_orelha[] = {COR_ANEIS_AMARELOS};
    const float cor_cont_anel_orelha[] = {COR_CONTORNO_ANEIS_AMARELOS};
    float anel_orelha_rx = larg_topo_seg_base_orelha * 0.6f;
    float anel_orelha_ry = 7.0f;
    float fator_espessura_anel_orelha = 0.35f;
    float anel_orelha_pos_y = comp_seg_base_orelha * 0.7f;
    desenhar_anel_fita_avancado(0.0f, anel_orelha_pos_y, anel_orelha_rx, anel_orelha_ry, anel_orelha_rx * (1.0f - fator_espessura_anel_orelha), anel_orelha_ry * (1.0f - fator_espessura_anel_orelha), NUM_SEGMENTOS_OVAL, cor_preench_anel_orelha, cor_cont_anel_orelha);
    glTranslatef(0.0f, comp_seg_base_orelha, 0.0f);
    glPushMatrix();
    glRotatef(angulo_ponta_atual, 0.0f, 0.0f, 1.0f);
    desenhar_segmento_simples(larg_base_seg_ponta_orelha, larg_topo_seg_ponta_orelha, comp_seg_ponta_orelha, cor_orelha, cor_contorno_orelha);
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
}

// Desenha uma perna ou braço do Umbreon
void desenhar_perna_umbreon(float pivo_x, float pivo_y, int lado, float ref_largura_topo_corpo, float altura_real_perna, float raio_externo_anel_perna, int is_arm) {
    float largura_topo_perna = 38.0f; float largura_base_perna = 30.0f;
    float pata_oval_rx = 19.0f; float pata_oval_ry = 13.0f;
    float pata_offset_y_da_base_perna = -pata_oval_ry * 0.3f;
    float ombro_offset_x = ref_largura_topo_corpo * 0.05f * lado;
    glPushMatrix();
    glTranslatef(pivo_x + ombro_offset_x, pivo_y, 0.0f);
    // Se for braço, aplica rotação para animação
    if (is_arm) {
        GLfloat angulo_braco = (lado == -1) ? g_angulo_braco_E : g_angulo_braco_D;
        glRotatef(lado * angulo_braco, 0.0f, 0.0f, 1.0f);
    }
    float vertices_perna[][2] = {
        {lado * (-largura_base_perna * 0.5f), 0},
        {lado * ( largura_base_perna * 0.5f), 0},
        {lado * ( largura_topo_perna * 0.4f), altura_real_perna},
        {lado * (-largura_topo_perna * 0.6f), altura_real_perna + 5}
    };
    glColor3f(COR_CORPO_UMBREON); desenhar_poligono_preenchido(vertices_perna, 4);
    glColor3f(COR_CONTORNO_CORPO_UMBREON); desenhar_contorno_poligono(vertices_perna, 4);
    glColor3f(COR_CORPO_UMBREON);
    desenhar_oval_preenchida(0, pata_offset_y_da_base_perna, pata_oval_rx, pata_oval_ry, NUM_SEGMENTOS_OVAL);
    glColor3f(COR_CONTORNO_CORPO_UMBREON);
    desenhar_contorno_oval(0, pata_offset_y_da_base_perna, pata_oval_rx, pata_oval_ry, NUM_SEGMENTOS_OVAL);
    // Anel amarelo na perna
    const float cor_preenchimento_anel[] = {COR_ANEIS_AMARELOS}; const float cor_contorno_anel[] = {COR_CONTORNO_ANEIS_AMARELOS};
    float anel_perna_rx = raio_externo_anel_perna; float anel_perna_ry = raio_externo_anel_perna * 0.70f;
    float fator_espessura_anel_perna = 0.35f;
    desenhar_anel_fita_avancado(0, altura_real_perna * 0.40f, anel_perna_rx, anel_perna_ry, anel_perna_rx * (1.0f - fator_espessura_anel_perna), anel_perna_ry * (1.0f - fator_espessura_anel_perna), NUM_SEGMENTOS_OVAL, cor_preenchimento_anel, cor_contorno_anel);
    glPopMatrix();
}

// Desenha o cenário de fundo (céu, chão, lua)
void desenhar_cenario() {
    glColor3f(COR_CEU_NOITE);
    glBegin(GL_QUADS);
    glVertex2f(0, altura_janela_global * 0.3f);
    glVertex2f(largura_janela_global, altura_janela_global * 0.3f);
    glVertex2f(largura_janela_global, altura_janela_global);
    glVertex2f(0, altura_janela_global);
    glEnd();
    glColor3f(COR_CHAO_NOITE);
    desenhar_oval_preenchida(largura_janela_global / 2.0f, altura_janela_global * 0.15f, largura_janela_global * 0.7f, altura_janela_global * 0.25f, NUM_SEGMENTOS_OVAL);
    glColor3f(COR_LUA);
    desenhar_oval_preenchida(largura_janela_global * 0.8f, altura_janela_global * 0.75f, 30.0f, 30.0f, NUM_SEGMENTOS_OVAL);
}

// ----------------------
// Função principal de desenho 
// ----------------------
void exibir() {
    // Parâmetros do corpo do Umbreon
    float umbreon_pivo_inicial_x = 320.0f; float umbreon_pivo_inicial_y = 160.0f;
    float cabeca_raio_x = 60.0f; float cabeca_raio_y = 58.0f; float cabeca_pivo_offset_y = 120.0f;
    float corpo_largura_base = 100.0f; float corpo_largura_topo = 70.0f; float corpo_altura = 105.0f;
    float orelha_comprimento = 100.0f; float orelha_pivo_x_na_cabeca = cabeca_raio_x * 0.45f; float orelha_pivo_y_na_cabeca = cabeca_raio_y * 0.5f;
    float olho_raio_x_base = 21.0f; float olho_raio_y_base = 18.0f; float olho_offset_x_do_centro = 26.0f; float olho_offset_y_do_centro_cabeca = 14.0f;
    float olho_brilho_rx_b = olho_raio_x_base * 0.20f; float olho_brilho_ry_base = olho_raio_y_base * 0.25f;
    float pupila_fator_rx = 0.33f; float pupila_fator_ry_base = 0.83f;
    float perna_anel_raio_externo = 17.0f; float perna_altura_param = corpo_altura * 0.65f;
    const float cor_preench_anel[] = {COR_ANEIS_AMARELOS}; const float cor_cont_anel[] = {COR_CONTORNO_ANEIS_AMARELOS};
    // Ajustes para piscada dos olhos
    float olho_raio_y_atual = olho_raio_y_base * g_fator_palpebra_y; float pupila_ry_atual = (olho_raio_y_base * pupila_fator_ry_base) * g_fator_palpebra_y; float olho_brilho_ry_atual = olho_brilho_ry_base * g_fator_palpebra_y; float brilho_pos_y_atual = (olho_raio_y_base*0.33f) * g_fator_palpebra_y;

    glClearColor(0.96f, 0.96f, 0.99f, 1.0f); glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_LINE_SMOOTH); glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); glHint(GL_LINE_SMOOTH_HINT, GL_NICEST); glLineWidth(LARGURA_LINHA_PADRAO);

    desenhar_cenario();

    // Aplica transformações globais (translação, escala, animação)
    glPushMatrix();
    glTranslatef(umbreon_pivo_inicial_x + g_deslocamento_x + g_danca_offset_x, umbreon_pivo_inicial_y + g_deslocamento_y + g_danca_offset_y, 0.0f);
    glScalef(g_escala_umbreon, g_escala_umbreon, 1.0f);
    glTranslatef(-umbreon_pivo_inicial_x, -umbreon_pivo_inicial_y, 0.0f);

    /* Ordem de desenho: 

     cauda, pernas traseiras, corpo, pernas dianteiras, cabeça

    */
    desenhar_cauda_umbreon_hierarquica(umbreon_pivo_inicial_x + 30.0f, umbreon_pivo_inicial_y + 10.5f);

    float perna_traseira_pivo_x_offset = corpo_largura_base * 0.20f; float perna_traseira_pivo_y_no_corpo = corpo_altura * 0.15f;
    desenhar_perna_umbreon(umbreon_pivo_inicial_x - perna_traseira_pivo_x_offset, umbreon_pivo_inicial_y + perna_traseira_pivo_y_no_corpo, -1, corpo_largura_topo, perna_altura_param * 0.9f, perna_anel_raio_externo * 0.9f, 0);
    desenhar_perna_umbreon(umbreon_pivo_inicial_x + perna_traseira_pivo_x_offset, umbreon_pivo_inicial_y + perna_traseira_pivo_y_no_corpo,  1, corpo_largura_topo, perna_altura_param * 0.9f, perna_anel_raio_externo * 0.9f, 0);

    // Corpo e pernas dianteiras (braços)
    glPushMatrix();
    glTranslatef(umbreon_pivo_inicial_x, umbreon_pivo_inicial_y, 0.0f);
    glRotatef(g_angulo_corpo, 0.0f, 0.0f, 1.0f);
    glTranslatef(-umbreon_pivo_inicial_x, -umbreon_pivo_inicial_y, 0.0f);

    glPushMatrix();
    glTranslatef(umbreon_pivo_inicial_x, umbreon_pivo_inicial_y, 0.0f);
    // Corpo (tronco)
    {
        float vertices_corpo[][2] = { {-corpo_largura_base / 2, 0}, { corpo_largura_base / 2, 0}, { corpo_largura_topo / 2,   corpo_altura}, {-corpo_largura_topo / 2,  corpo_altura} };
        glColor3f(COR_CORPO_UMBREON); desenhar_poligono_preenchido(vertices_corpo, 4);
        glColor3f(COR_CONTORNO_CORPO_UMBREON); desenhar_contorno_poligono(vertices_corpo, 4);
        // Anéis amarelos no corpo
        float anel_corpo_pos_y = corpo_altura * 0.18f; float anel_corpo_offset_x = corpo_largura_base * 0.38f;
        float anel_marca_rx = perna_anel_raio_externo * 0.85f, anel_marca_ry = perna_anel_raio_externo * 1.05f;
        float fator_espessura_marca = 0.35f;
        desenhar_anel_fita_avancado(-anel_corpo_offset_x, anel_corpo_pos_y, anel_marca_rx, anel_marca_ry, anel_marca_rx*(1-fator_espessura_marca), anel_marca_ry*(1-fator_espessura_marca), NUM_SEGMENTOS_OVAL, cor_preench_anel, cor_cont_anel);
        desenhar_anel_fita_avancado( anel_corpo_offset_x, anel_corpo_pos_y, anel_marca_rx, anel_marca_ry, anel_marca_rx*(1-fator_espessura_marca), anel_marca_ry*(1-fator_espessura_marca), NUM_SEGMENTOS_OVAL, cor_preench_anel, cor_cont_anel);
    }
    glPopMatrix();

    // Pernas dianteiras (braços)
    float perna_pivo_base_y = umbreon_pivo_inicial_y - 12.f; float perna_espacamento_do_centro = corpo_largura_topo * 0.40f;
    desenhar_perna_umbreon(umbreon_pivo_inicial_x - perna_espacamento_do_centro, perna_pivo_base_y, -1, corpo_largura_topo, perna_altura_param, perna_anel_raio_externo, 1);
    desenhar_perna_umbreon(umbreon_pivo_inicial_x + perna_espacamento_do_centro, perna_pivo_base_y,  1, corpo_largura_topo, perna_altura_param, perna_anel_raio_externo, 1);

    glPopMatrix();

    // Cabeça e detalhes
    glPushMatrix();
    glTranslatef(umbreon_pivo_inicial_x, umbreon_pivo_inicial_y + cabeca_pivo_offset_y, 0.0f);
    glRotatef(g_angulo_cabeca, 0.0f, 0.0f, 1.0f);

    // Orelhas
    desenhar_orelha_umbreon_hierarquica(orelha_pivo_x_na_cabeca, orelha_pivo_y_na_cabeca, orelha_comprimento, -1);
    desenhar_orelha_umbreon_hierarquica(orelha_pivo_x_na_cabeca, orelha_pivo_y_na_cabeca, orelha_comprimento,  1);

    // Cabeça (oval)
    glColor3f(COR_CORPO_UMBREON); desenhar_oval_preenchida(0, 0, cabeca_raio_x, cabeca_raio_y, NUM_SEGMENTOS_OVAL);
    glColor3f(COR_CONTORNO_CORPO_UMBREON); desenhar_contorno_oval(0, 0, cabeca_raio_x, cabeca_raio_y, NUM_SEGMENTOS_OVAL);

    // Anel amarelo na testa
    float anel_testa_rx = cabeca_raio_x * 0.48f, anel_testa_ry = cabeca_raio_y * 0.28f;
    float fator_espessura_anel_testa = 0.35f;
    desenhar_anel_fita_avancado(0, cabeca_raio_y * 0.42f, anel_testa_rx, anel_testa_ry, anel_testa_rx*(1-fator_espessura_anel_testa), anel_testa_ry*(1-fator_espessura_anel_testa), NUM_SEGMENTOS_OVAL, cor_preench_anel, cor_cont_anel);

    // Olhos (esquerdo e direito)
    glPushMatrix(); glTranslatef(-olho_offset_x_do_centro, olho_offset_y_do_centro_cabeca, 0.0f);
    glColor3f(COR_OLHO_VERMELHO); desenhar_oval_preenchida(0,0, olho_raio_x_base, olho_raio_y_atual, NUM_SEGMENTOS_OVAL);
    glColor3f(COR_CONTORNO_OLHO); desenhar_contorno_oval(0,0, olho_raio_x_base, olho_raio_y_atual, NUM_SEGMENTOS_OVAL);
    if (g_fator_palpebra_y > 0.051f) {
        glColor3f(COR_PUPILA_OLHO); desenhar_oval_preenchida(0,0, olho_raio_x_base * pupila_fator_rx, pupila_ry_atual, NUM_SEGMENTOS_OVAL);
        glColor3f(COR_BRILHO_OLHO); desenhar_oval_preenchida(olho_raio_x_base*0.23f, brilho_pos_y_atual, olho_brilho_rx_b, olho_brilho_ry_atual, NUM_SEGMENTOS_OVAL);
    }
    glPopMatrix();

    glPushMatrix(); glTranslatef(olho_offset_x_do_centro, olho_offset_y_do_centro_cabeca, 0.0f);
    glColor3f(COR_OLHO_VERMELHO); desenhar_oval_preenchida(0,0, olho_raio_x_base, olho_raio_y_atual, NUM_SEGMENTOS_OVAL);
    glColor3f(COR_CONTORNO_OLHO); desenhar_contorno_oval(0,0, olho_raio_x_base, olho_raio_y_atual, NUM_SEGMENTOS_OVAL);
    if (g_fator_palpebra_y > 0.051f) {
        glColor3f(COR_PUPILA_OLHO); desenhar_oval_preenchida(0,0, olho_raio_x_base * pupila_fator_rx, pupila_ry_atual, NUM_SEGMENTOS_OVAL);
        glColor3f(COR_BRILHO_OLHO); desenhar_oval_preenchida(olho_raio_x_base*0.23f, brilho_pos_y_atual, olho_brilho_rx_b, olho_brilho_ry_atual, NUM_SEGMENTOS_OVAL);
    }
    glPopMatrix();

    glPopMatrix(); 
    glPopMatrix(); 

    glFlush();
}

// ----------------------
// Função de reshape (ajusta viewport e projeção)
// ----------------------
void reshape(int w, int h) {
    largura_janela_global = w;
    altura_janela_global = h;
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (h == 0) h = 1;
    gluOrtho2D(0.0, (GLdouble)w, 0.0, (GLdouble)h);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// ----------------------
// Função para resetar animação ao modo parado
// ----------------------
void resetar_animacao() {
    g_angulo_cabeca = 0.0f; g_angulo_orelha_E_base = 0.0f; g_angulo_orelha_D_base = 0.0f;
    g_angulo_cauda_base = 0.0f; g_angulo_cauda_meio = 0.0f; g_angulo_cauda_ponta = 10.0f;
    g_angulo_braco_E = 0.0f; g_angulo_braco_D = 0.0f; g_angulo_corpo = 0.0f;
    g_danca_offset_x = 0.0f; g_danca_offset_y = 0.0f;
    g_fase_coreografia = 0; g_tempo_fase = 0;
}

// ----------------------
// Lógica de animação e coreografias
// ----------------------
void animacao_loop(int valor) {
    //  piscada dos olhos
    g_tempo_piscada_contador++;
    switch (g_estado_piscada) {
        case 0: if (g_tempo_piscada_contador >= INTERVALO_ENTRE_PISCADAS) { g_estado_piscada = 1; g_tempo_piscada_contador = 0; } break;
        case 1: g_fator_palpebra_y -= (1.0f / (float)DURACAO_FASE_PISCADA); if (g_fator_palpebra_y <= 0.0f) { g_fator_palpebra_y = 0.0f; g_estado_piscada = 2; g_tempo_piscada_contador = 0; } break;
        case 2: if (g_tempo_piscada_contador >= TEMPO_OLHOS_FECHADOS) { g_estado_piscada = 3; g_tempo_piscada_contador = 0; } break;
        case 3: g_fator_palpebra_y += (1.0f / (float)DURACAO_FASE_PISCADA); if (g_fator_palpebra_y >= 1.0f) { g_fator_palpebra_y = 1.0f; g_estado_piscada = 0; g_tempo_piscada_contador = 0; } break;
    }
    g_tempo_animacao += 0.05f;

    // Modos de animação
    switch (g_modo_animacao) {
        case 0: // Parado (respiração)
            g_danca_offset_y = 2.5f * fabs(sin(g_tempo_animacao * 0.75f));
            g_orelha_E_base_alvo = g_angulo_orelha_E_base;
            g_orelha_D_base_alvo = g_angulo_orelha_D_base;
            g_orelha_E_ponta_alvo = g_angulo_orelha_E_ponta;
            g_orelha_D_ponta_alvo = g_angulo_orelha_D_ponta;
            break;
        case 1: // Balada
            g_angulo_cabeca = 15.0f * sin(g_tempo_animacao * 2.0f);
            g_angulo_cauda_base = 25.0f * sin(g_tempo_animacao * 3.0f);
            g_angulo_cauda_meio = 35.0f * sin(g_tempo_animacao * 3.0f + 1.0f);
            g_angulo_cauda_ponta = 35.0f * sin(g_tempo_animacao * 3.0f + 2.0f);
            g_angulo_braco_E = 45.0f * fabs(sin(g_tempo_animacao * 2.5f + 0.5f));
            g_angulo_braco_D = 45.0f * fabs(sin(g_tempo_animacao * 2.5f));
            g_danca_offset_y = 15.0f * fabs(sin(g_tempo_animacao * 2.0f));
            g_danca_offset_x = 20.0f * sin(g_tempo_animacao * 1.5f);
            g_orelha_E_base_alvo = 0.7f * g_angulo_cabeca + 10.0f * sin(g_tempo_animacao * 2.0f + 0.5f);
            g_orelha_D_base_alvo = 0.7f * g_angulo_cabeca + 10.0f * sin(g_tempo_animacao * 2.0f);
            break;
        case 2: // Jogar de ladin 
            g_angulo_corpo = -25.0f;
            g_angulo_cabeca = -10.0f + 4.0f * sin(g_tempo_animacao * 2.0f);
            g_angulo_braco_E = 60.0f + 8.0f * sin(g_tempo_animacao * 2.5f);
            g_angulo_braco_D = 60.0f + 8.0f * cos(g_tempo_animacao * 2.5f);
            g_danca_offset_x = 0;
            g_danca_offset_y = 2.0f * fabs(sin(g_tempo_animacao * 1.5f));
            g_angulo_cauda_base = 5.0f * sin(g_tempo_animacao * 2.0f);
            g_orelha_E_base_alvo = 5.0f * sin(g_tempo_animacao * 2.0f);
            g_orelha_D_base_alvo = 5.0f * sin(g_tempo_animacao * 2.0f + 1.0f);
            break;
        case 3: // Dancinha
            g_tempo_fase++;
            if (g_fase_coreografia == 0) {
                g_angulo_corpo = 10.0f * sin(g_tempo_animacao * 3.0f);
                g_angulo_braco_E = 60.0f * sin(g_tempo_animacao * 4.0f);
                g_angulo_braco_D = 60.0f * cos(g_tempo_animacao * 4.0f);
                g_angulo_cauda_base = 30.0f * sin(g_tempo_animacao * 3.0f);
                g_angulo_cauda_meio = 30.0f * sin(g_tempo_animacao * 3.0f + 1.0f);
                g_danca_offset_y = 0;
                g_orelha_E_base_alvo = 18.0f * sin(g_tempo_animacao * 2.0f);
                g_orelha_D_base_alvo = -18.0f * sin(g_tempo_animacao * 2.0f);
                if (g_tempo_fase > 80) { g_fase_coreografia = 1; g_tempo_fase = 0; }
            } else if (g_fase_coreografia == 1) {
                if (g_tempo_fase < 5) {
                    g_danca_offset_y = -15.0f; g_angulo_braco_E = 30.0f; g_angulo_braco_D = 30.0f; g_angulo_cabeca = 10.0f;
                    float impacto = sin(g_tempo_fase * 3.0f) * exp(-g_tempo_fase * 0.25f);
                    g_orelha_E_base_alvo = impacto * 35.0f;
                    g_orelha_D_base_alvo = impacto * 35.0f;
                } else {
                    g_danca_offset_y = 0; g_angulo_cabeca = 0.0f;
                    g_orelha_E_base_alvo = 0.0f;
                    g_orelha_D_base_alvo = 0.0f;
                }
                if (g_tempo_fase > 20) { g_fase_coreografia = 2; g_tempo_fase = 0; }
            } else if (g_fase_coreografia == 2) {
                g_angulo_corpo = 25.0f * sin(g_tempo_animacao * 5.0f);
                g_angulo_cabeca = -g_angulo_corpo;
                g_angulo_braco_D = 20.0f * (1 + sin(g_tempo_animacao * 5.0f));
                g_angulo_braco_E = 0;
                g_danca_offset_y = 0;
                g_orelha_E_base_alvo = 0.9f * g_angulo_corpo + 10.0f * sin(g_tempo_animacao * 2.0f);
                g_orelha_D_base_alvo = 0.9f * g_angulo_corpo + 10.0f * sin(g_tempo_animacao * 2.0f + 1.0f);
                if (g_tempo_fase > 80) { g_fase_coreografia = 0; g_tempo_fase = 0; }
            }
            break;
    }

    // Suavização das orelhas 
    g_angulo_orelha_E_base = g_angulo_orelha_E_base * (1.0f - ORELHA_BASE_SUAVIZACAO) + g_orelha_E_base_alvo * ORELHA_BASE_SUAVIZACAO;
    g_angulo_orelha_D_base = g_angulo_orelha_D_base * (1.0f - ORELHA_BASE_SUAVIZACAO) + g_orelha_D_base_alvo * ORELHA_BASE_SUAVIZACAO;

    if (g_modo_animacao == 0) {
        g_angulo_orelha_E_ponta = g_angulo_orelha_E_ponta * (1.0f - ORELHA_PONTA_SUAVIZACAO) + g_orelha_E_ponta_alvo * ORELHA_PONTA_SUAVIZACAO;
        g_angulo_orelha_D_ponta = g_angulo_orelha_D_ponta * (1.0f - ORELHA_PONTA_SUAVIZACAO) + g_orelha_D_ponta_alvo * ORELHA_PONTA_SUAVIZACAO;
    } else {
        g_orelha_E_ponta_alvo = g_angulo_orelha_E_base * 1.18f;
        g_orelha_D_ponta_alvo = g_angulo_orelha_D_base * 1.18f;
        g_angulo_orelha_E_ponta = g_angulo_orelha_E_ponta * (1.0f - ORELHA_PONTA_SUAVIZACAO) + g_orelha_E_ponta_alvo * ORELHA_PONTA_SUAVIZACAO;
        g_angulo_orelha_D_ponta = g_angulo_orelha_D_ponta * (1.0f - ORELHA_PONTA_SUAVIZACAO) + g_orelha_D_ponta_alvo * ORELHA_PONTA_SUAVIZACAO;
    }

    glutPostRedisplay();
    glutTimerFunc(16, animacao_loop, 0);
}

// ----------------------
// Gerenciamento de teclado (letras)
// ----------------------
void gerenciar_teclado(unsigned char tecla, int x, int y) {
    // Se estiver em modo animado, qualquer tecla manual volta ao modo parado
    if (g_modo_animacao != 0 && tecla != 'p' && tecla != 'P' && tecla != 'l' && tecla != 'L' && tecla != 'c' && tecla != 'C') {
        g_modo_animacao = 0;
        resetar_animacao();
    }
    switch (tecla) {
        case 27: exit(0); break; 
        // Modos de animação
        case 'p': case 'P': g_modo_animacao = (g_modo_animacao == 1) ? 0 : 1; if (g_modo_animacao == 0) resetar_animacao(); break;
        case 'l': case 'L': g_modo_animacao = (g_modo_animacao == 2) ? 0 : 2; if (g_modo_animacao == 0) resetar_animacao(); break;
        case 'c': case 'C': g_modo_animacao = (g_modo_animacao == 3) ? 0 : 3; if (g_modo_animacao == 0) resetar_animacao(); break;
        // Controles manuais das articulações e escala
        case 'q': case 'Q': { GLfloat angulo_atual = g_angulo_cauda_base; GLfloat proximo_angulo = angulo_atual + g_incremento_angulo_cauda; if (angulo_atual <= ZONA_PROIBIDA_CAUDA_INICIO && proximo_angulo > ZONA_PROIBIDA_CAUDA_INICIO) { g_angulo_cauda_base = ZONA_PROIBIDA_CAUDA_INICIO; } else { g_angulo_cauda_base = proximo_angulo; } break; }
        case 'a': case 'A': { GLfloat angulo_atual = g_angulo_cauda_base; GLfloat proximo_angulo = angulo_atual - g_incremento_angulo_cauda; if (angulo_atual >= ZONA_PROIBIDA_CAUDA_FIM && proximo_angulo < ZONA_PROIBIDA_CAUDA_FIM) { g_angulo_cauda_base = ZONA_PROIBIDA_CAUDA_FIM; } else { g_angulo_cauda_base = proximo_angulo; } break; }
        case 'w': case 'W': g_angulo_cauda_meio += g_incremento_angulo_cauda; if (g_angulo_cauda_meio > LIMITE_CAUDA_MEIO) { g_angulo_cauda_meio = LIMITE_CAUDA_MEIO; } break;
        case 's': case 'S': g_angulo_cauda_meio -= g_incremento_angulo_cauda; if (g_angulo_cauda_meio < -LIMITE_CAUDA_MEIO) { g_angulo_cauda_meio = -LIMITE_CAUDA_MEIO; } break;
        case 'e': case 'E': g_angulo_cauda_ponta += g_incremento_angulo_cauda; if (g_angulo_cauda_ponta > LIMITE_CAUDA_PONTA) { g_angulo_cauda_ponta = LIMITE_CAUDA_PONTA; } break;
        case 'd': case 'D': g_angulo_cauda_ponta -= g_incremento_angulo_cauda; if (g_angulo_cauda_ponta < -LIMITE_CAUDA_PONTA) { g_angulo_cauda_ponta = -LIMITE_CAUDA_PONTA; } break;
        case 'r': case 'R': g_angulo_orelha_E_base += g_incremento_angulo_orelha; if (g_angulo_orelha_E_base > LIMITE_ANGULO_ORELHA_BASE) { g_angulo_orelha_E_base = LIMITE_ANGULO_ORELHA_BASE; } break;
        case 'f': case 'F': g_angulo_orelha_E_base -= g_incremento_angulo_orelha; if (g_angulo_orelha_E_base < -LIMITE_ANGULO_ORELHA_BASE) { g_angulo_orelha_E_base = -LIMITE_ANGULO_ORELHA_BASE; } break;
        case 't': case 'T': g_angulo_orelha_E_ponta += g_incremento_angulo_orelha; if (g_angulo_orelha_E_ponta > LIMITE_ANGULO_ORELHA_PONTA) { g_angulo_orelha_E_ponta = LIMITE_ANGULO_ORELHA_PONTA; } break;
        case 'g': case 'G': g_angulo_orelha_E_ponta -= g_incremento_angulo_orelha; if (g_angulo_orelha_E_ponta < -LIMITE_ANGULO_ORELHA_PONTA) { g_angulo_orelha_E_ponta = -LIMITE_ANGULO_ORELHA_PONTA; } break;
        case 'y': case 'Y': g_angulo_orelha_D_base += g_incremento_angulo_orelha; if (g_angulo_orelha_D_base > LIMITE_ANGULO_ORELHA_BASE) { g_angulo_orelha_D_base = LIMITE_ANGULO_ORELHA_BASE; } break;
        case 'h': case 'H': g_angulo_orelha_D_base -= g_incremento_angulo_orelha; if (g_angulo_orelha_D_base < -LIMITE_ANGULO_ORELHA_BASE) { g_angulo_orelha_D_base = -LIMITE_ANGULO_ORELHA_BASE; } break;
        case 'u': case 'U': g_angulo_orelha_D_ponta += g_incremento_angulo_orelha; if (g_angulo_orelha_D_ponta > LIMITE_ANGULO_ORELHA_PONTA) { g_angulo_orelha_D_ponta = LIMITE_ANGULO_ORELHA_PONTA; } break;
        case 'j': case 'J': g_angulo_orelha_D_ponta -= g_incremento_angulo_orelha; if (g_angulo_orelha_D_ponta < -LIMITE_ANGULO_ORELHA_PONTA) { g_angulo_orelha_D_ponta = -LIMITE_ANGULO_ORELHA_PONTA; } break;
        case 'z': case 'Z': g_angulo_cabeca += g_incremento_angulo_cabeca; if (g_angulo_cabeca > LIMITE_ANGULO_CABECA) { g_angulo_cabeca = LIMITE_ANGULO_CABECA; } break;
        case 'x': case 'X': g_angulo_cabeca -= g_incremento_angulo_cabeca; if (g_angulo_cabeca < -LIMITE_ANGULO_CABECA) { g_angulo_cabeca = -LIMITE_ANGULO_CABECA; } break;
        case 'm': case 'M': g_escala_umbreon += g_incremento_escala; if (g_escala_umbreon > ESCALA_MAXIMA) { g_escala_umbreon = ESCALA_MAXIMA; } break;
        case 'n': case 'N': g_escala_umbreon -= g_incremento_escala; if (g_escala_umbreon < ESCALA_MINIMA) { g_escala_umbreon = ESCALA_MINIMA; } break;
    }
    glutPostRedisplay();
}

// ----------------------
// Gerenciamento de teclas especiais (setas, page up/down)
// ----------------------
void gerenciar_teclas_especiais(int tecla, int x, int y) {
    // Se estiver em modo animado, qualquer tecla manual volta ao modo parado
    if (g_modo_animacao != 0) {
        g_modo_animacao = 0;
        resetar_animacao();
    }
    switch (tecla) {
        case GLUT_KEY_UP:    g_deslocamento_y += g_incremento_deslocamento; break;
        case GLUT_KEY_DOWN:  g_deslocamento_y -= g_incremento_deslocamento; break;
        case GLUT_KEY_LEFT:  g_deslocamento_x -= g_incremento_deslocamento; break;
        case GLUT_KEY_RIGHT: g_deslocamento_x += g_incremento_deslocamento; break;
        case GLUT_KEY_PAGE_UP: g_escala_umbreon += g_incremento_escala; if (g_escala_umbreon > ESCALA_MAXIMA) { g_escala_umbreon = ESCALA_MAXIMA; } break;
        case GLUT_KEY_PAGE_DOWN: g_escala_umbreon -= g_incremento_escala; if (g_escala_umbreon < ESCALA_MINIMA) { g_escala_umbreon = ESCALA_MINIMA; } break;
    }
    glutPostRedisplay();
}

// ----------------------
// Função principal (main)
// ----------------------
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(largura_janela_global, altura_janela_global);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Umbreon dançarino");

    glutDisplayFunc(exibir);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(gerenciar_teclado);
    glutSpecialFunc(gerenciar_teclas_especiais);
    glutTimerFunc(16, animacao_loop, 0);

    glutMainLoop();

    return 0;
}