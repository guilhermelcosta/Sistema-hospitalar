/**
 * @file main.c
 * @author Guilherme Costa, Lucas Soares e Vitor Lagares.
 * @brief Esse programa funciona como um sistema hospitalar de gerenciamento de pacientes, medicos e consultas.
 * @version 1.0
 * @date 2022-12-08
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include <conio.h>

struct PACIENTE
{
    char codigo[7];
    char nome[50];
    char endereco[50];
    char telefone[16];
    char nascimento[20];
};

struct MEDICO
{
    char codigo[7];
    char nome[50];
    char telefone[16];
    char especialidade[20];
};

struct CONSULTA
{
    char codigo[7];
    char data[11];
    char hora[6];
    char codigoPaciente[7];
    char nomePaciente[50];
    char codigoMedico[7];
    char nomeMedico[50];
};

// Emissao de relatorios:
void exibeConsultasNaData();
void exibeConsultasRealizadasAteDataCorrente();
void exibeConsultasMedicos();
int converteDataEmInt();

// Gerador de sequencias numericas aleatorias:
int geraCodigo();

// Funcionalidades relacionadas ao paciente:
void cadastrarPaciente();
void exibePacientes();
int disponibilidadeCodigo_Paciente(FILE *lista_pacientes, char[7]);

// Funcionalidades relacionadas ao medico:
void cadastrarMedico();
void exibeMedicos();
int disponibilidadeCodigo_Medico(FILE *lista_medicos, char[7]);

// Funcionalidades relacionadas as consultas:
void cadastrarConsulta();
void cancelarConsulta();
void exibeConsultas();
int disponibilidadeCodigo_Consulta(FILE *lista_consultas, char[7]);
int confereDisponilidadeDataSugerida(FILE *lista_consultas, char[11], char[7]);
int confereDisponilidadeHorarioSugerido(FILE *lista_consultas, char[6], char[7], char[11]);
int converteHorarioEmSeg(char[6]);

int main()
{
    setlocale(LC_ALL, "portuguese");
    srand(time(NULL));

    char op;

    do
    {
        printf("\n*************************************************************\n");
        printf("\n Selecione a acao desejada:                                 *\n");
        printf("\n A - CADASTRAR NOVO PACIENTE                                *\n");
        printf("\n B - CADASTRAR NOVO MEDICO                                  *\n");
        printf("\n C - CADASTRAR CONSULTA                                     *\n");
        printf("\n D - CANCELAR UMA CONSULTA                                  *\n");
        printf("\n E - EXIBIR PACIENTES CADASTRADOS                           *\n");
        printf("\n F - EXIBIR MEDICOS CADASTRADOS                             *\n");
        printf("\n G - EXIBIR CONSULTAS CADASTRADAS                           *\n");
        printf("\n H - PROCURAR CONSULTAS EM DATA ESPECIFICA                  *\n");
        printf("\n I - PROCURAR CONSULTAS REALIZADAS ATE DATA CORRENTE        *\n");
        printf("\n J - PROCURAR CONSULTAS POR MEDICO                          *\n");
        printf("\n K - SAIR                                                   *\n");
        printf("\n*************************************************************\n");

        op = getch();

        switch (op)
        {
        case 'A':
            printf("\n*************************************************************\n");
            printf("\n                   CADASTRAR NOVO PACIENTE                   \n");
            printf("\n*************************************************************\n");
            cadastrarPaciente();
            system("pause");
            system("cls");
            break;
        case 'B':
            printf("\n*************************************************************\n");
            printf("\n                    CADASTRAR NOVO MEDICO                    \n");
            printf("\n*************************************************************\n");
            cadastrarMedico();
            system("pause");
            system("cls");
            break;
        case 'C':
            printf("\n*************************************************************\n");
            printf("\n                   CADASTRAR NOVA CONSULTA                   \n");
            printf("\n*************************************************************\n");
            cadastrarConsulta();
            system("pause");
            system("cls");
            break;
        case 'D':
            printf("\n*************************************************************\n");
            printf("\n                    CANCELAR UMA CONSULTA                    \n");
            printf("\n*************************************************************\n");
            cancelarConsulta();
            system("pause");
            system("cls");
            break;
        case 'E':
            printf("\n*************************************************************\n");
            printf("\n                 EXIBIR PACIENTES CADASTRADOS                \n");
            printf("\n*************************************************************\n");
            exibePacientes();
            system("pause");
            system("cls");
            break;
        case 'F':
            printf("\n*************************************************************\n");
            printf("\n                  EXIBIR MEDICOS CADASTRADOS                 \n");
            printf("\n*************************************************************\n");
            exibeMedicos();
            system("pause");
            system("cls");
            break;
        case 'G':
            printf("\n*************************************************************\n");
            printf("\n                 EXIBIR CONSULTAS CADASTRADAS                \n");
            printf("\n*************************************************************\n");
            exibeConsultas();
            system("pause");
            system("cls");
            break;
        case 'H':
            printf("\n*************************************************************\n");
            printf("\n            PROCURAR CONSULTAS EM DATA ESPECIFICA            \n");
            printf("\n*************************************************************\n");
            exibeConsultasNaData();
            system("pause");
            system("cls");
            break;
        case 'I':
            printf("\n*************************************************************\n");
            printf("\n      PROCURAR CONSULTAS REALIZADAS ATE DATA CORRENTE        \n");
            printf("\n*************************************************************\n");
            exibeConsultasRealizadasAteDataCorrente();
            system("pause");
            system("cls");
            break;
        case 'J':
            printf("\n*************************************************************\n");
            printf("\n               PROCURAR CONSULTAS POR MEDICO                 \n");
            printf("\n*************************************************************\n");
            exibeConsultasMedicos();
            system("pause");
            system("cls");
            break;
        default:
            break;
        }
    } while (op != 'K');
    return 0;
}
// --------------------------------------------------------------------------------
// GERADOR DE SEQUENCIAS NUMERAS ALEATORIAS PARA CODIGOS.
// --------------------------------------------------------------------------------
/**
 * @brief Funcao responsavel por gerar a parte numerica dos codigos (4 digitos), para os pacientes, medicos e consultas.
 *
 * @return int
 */
int geraCodigo()
{
    int sequenciaNumerica;

    // Considera valores entre 1000 e 9999.
    sequenciaNumerica = (rand() % 9000) + 1000;

    return sequenciaNumerica;
}

// --------------------------------------------------------------------------------
// FUNCOES RELACIONADAS AO PACIENTE.
// --------------------------------------------------------------------------------
/**
 * @brief Funcao responsavel pelo cadastro de um novo paciente.
 *
 */
void cadastrarPaciente()
{
    struct PACIENTE paciente;
    FILE *lista_pacientes;

    int sequenciaNumerica, cont = 0, limiteAtingido = 0;
    char codigoString[7];

    if ((lista_pacientes = fopen("lista_pacientes.txt", "r+b")) == NULL)
    {
        printf("\nArquivo nao existente, gerando arquivo base.\n");

        if ((lista_pacientes = fopen("lista_pacientes.txt", "w+b")) == NULL)
        {
            printf("\nErro na criacao do arquivo.\n");
            exit(1);
        }
        system("pause");
    }

    // [1] Gera a sequencia numerica do codigo, o converte em string, e em seguida adiciona o digito relativo de usuario 'P'.
    sequenciaNumerica = geraCodigo();
    sprintf(codigoString, "%i", sequenciaNumerica);
    strcat(codigoString, "-P");
    // Verifica se o codigo atribuido ao paciente ja foi utilizado. Se sim, refaz a operacao.
    while (disponibilidadeCodigo_Paciente(lista_pacientes, codigoString) != -1)
    {
        sequenciaNumerica = geraCodigo();
        // Verificar comentario [1].
        sprintf(codigoString, "%i", sequenciaNumerica);
        strcat(codigoString, "-P");
        cont++;

        // O valor de 9000 e relativo ao numero maximo de combinacoes (de 9.999 a 1.000).
        if (cont == 9000)
        {
            printf("\nLimite de pacientes atingido, operacao cancelada. Entre em contato com o administrador sistema para verificar o problema.\n");
            limiteAtingido = !limiteAtingido;
            break;
        }
    }

    // Apenas cadastra um novo paciente se nao tivermos atingidos o limite de codigos disponiveis (9.000).
    if (!limiteAtingido)
    {
        strcpy(paciente.codigo, codigoString);
        printf("\nCodigo do paciente: %s\n", paciente.codigo);
        printf("\nDigite o nome do paciente: ");
        fflush(stdin);
        gets(paciente.nome);
        printf("\nDigite o endereco do paciente: ");
        fflush(stdin);
        gets(paciente.endereco);
        printf("\nDigite o e telefone do paciente ((ddd) 00000-0000): ");
        fflush(stdin);
        gets(paciente.telefone);
        printf("\nDigite a data de nascimento do paciente (dd/mm/aaaa): ");
        fflush(stdin);
        gets(paciente.nascimento);

        // Salva as informacoes no arquivo, caso o nome nao esteja em branco.
        if (paciente.nome[0] != '\0')
        {
            fseek(lista_pacientes, 0, SEEK_END);
            fwrite(&paciente, sizeof(struct PACIENTE), 1, lista_pacientes);
            fclose(lista_pacientes);
            printf("\n*Paciente cadastrado com sucesso!*\n\n");
        }
        else
            printf("\n*Informacoes insuficientes. Operacao cancelada.*\n\n");
    }
}

/**
 * @brief Funcao que verifica se o codigo do indicado ja esta ou nao sendo utilizado por outro paciente.
 *
 * @param lista_pacientes
 * @param codigo
 * @return int
 */
int disponibilidadeCodigo_Paciente(FILE *lista_pacientes, char codigo[7])
{
    struct PACIENTE paciente;

    int posicao = 1, achou = 0;

    fseek(lista_pacientes, 0, SEEK_SET);
    fread(&paciente, sizeof(struct PACIENTE), 1, lista_pacientes);

    while (!feof(lista_pacientes) && !achou)
    {
        posicao++;

        if (strcmp(paciente.codigo, codigo) == 0)
            achou = 1;

        fread(&paciente, sizeof(struct PACIENTE), 1, lista_pacientes);
    }

    if (achou)
        return 1;
    else
        return -1;
}

/**
 * @brief Exibe os pacientes cadastrados no sistema.
 *
 */
void exibePacientes()
{
    struct PACIENTE paciente;
    FILE *lista_pacientes;

    if ((lista_pacientes = fopen("lista_pacientes.txt", "r+b")) != NULL)
    {
        fread(&paciente, sizeof(struct PACIENTE), 1, lista_pacientes);
        while (!feof(lista_pacientes))
        {
            printf("\n");
            printf("Codigo: %s\n", paciente.codigo);
            printf("Nome: %s\n", paciente.nome);
            printf("Endereco: %s\n", paciente.endereco);
            printf("Telefone: %s\n", paciente.telefone);
            printf("Nascimento: %s\n", paciente.nascimento);
            printf("\n");

            fread(&paciente, sizeof(struct PACIENTE), 1, lista_pacientes);
        }
    }
    else
        printf("\nErro na abertura do arquivo, a lista de pacientes nao pode ser exibida.\n");

    fclose(lista_pacientes);
}

// --------------------------------------------------------------------------------
// FUNCOES RELACIONADAS AO MEDICO.
// --------------------------------------------------------------------------------
/**
 * @brief Funcao responsavel pelo cadastro de um novo medico.
 *
 */
void cadastrarMedico()
{
    struct MEDICO medico;
    FILE *lista_medicos;

    int sequenciaNumerica, cont = 0, limiteAtingido = 0;
    char codigoString[7];

    if ((lista_medicos = fopen("lista_medicos.txt", "r+b")) == NULL)
    {
        printf("\nArquivo nao existente, gerando arquivo base.\n");

        if ((lista_medicos = fopen("lista_medicos.txt", "w+b")) == NULL)
        {
            printf("\nErro na criacao do arquivo.\n");
            exit(1);
        }
        system("pause");
    }

    // [1] Gera a sequencia numerica do codigo, o converte em string, e em seguida adiciona o digito relativo de usuario 'P'.
    sequenciaNumerica = geraCodigo();
    sprintf(codigoString, "%i", sequenciaNumerica);
    strcat(codigoString, "-M");
    // Verifica se o codigo atribuido ao medico ja foi utilizado. Se sim, refaz a operacao.
    while (disponibilidadeCodigo_Medico(lista_medicos, codigoString) != -1)
    {
        sequenciaNumerica = geraCodigo();
        // Verificar comentario [1].
        sprintf(codigoString, "%i", sequenciaNumerica);
        strcat(codigoString, "-M");
        cont++;

        // O valor de 9000 e relativo ao numero maximo de combinacoes (de 9.999 a 1.000).
        if (cont == 9000)
        {
            printf("\nLimite de medicos atingido, operacao cancelada. Entre em contato com o administrador sistema para verificar o problema.\n");
            limiteAtingido = !limiteAtingido;
            break;
        }
    }

    // Apenas cadastra um novo medico se nao tivermos atingidos o limite de codigos disponiveis (9.000).
    if (!limiteAtingido)
    {
        strcpy(medico.codigo, codigoString);
        printf("\nCodigo do medico: %s\n", medico.codigo);
        printf("\nDigite o nome do medico: ");
        fflush(stdin);
        gets(medico.nome);
        printf("\nDigite o e telefone do medico ((ddd) 00000-0000): ");
        fflush(stdin);
        gets(medico.telefone);
        printf("\nDigite a especialidade do medico: ");
        fflush(stdin);
        gets(medico.especialidade);

        // Salva as informacoes no arquivo, caso o nome nao esteja em branco.
        if (medico.nome[0] != '\0')
        {
            fseek(lista_medicos, 0, SEEK_END);
            fwrite(&medico, sizeof(struct MEDICO), 1, lista_medicos);
            fclose(lista_medicos);
            printf("\n*Medico cadastrado com sucesso!*\n\n");
        }
        else
            printf("\n*Informacoes insuficientes. Operacao cancelada.*\n\n");
    }
}

/**
 * @brief Funcao que verifica se o codigo do indicado ja esta ou nao sendo utilizado por outro medico.
 *
 * @param lista_medicos
 * @param codigo
 * @return int
 */
int disponibilidadeCodigo_Medico(FILE *lista_medicos, char codigo[7])
{
    struct MEDICO medico;

    int posicao = 1, achou = 0;

    fseek(lista_medicos, 0, SEEK_SET);
    fread(&medico, sizeof(struct MEDICO), 1, lista_medicos);

    while (!feof(lista_medicos) && !achou)
    {
        posicao++;

        if (strcmp(medico.codigo, codigo) == 0)
            achou = 1;

        fread(&medico, sizeof(struct MEDICO), 1, lista_medicos);
    }

    if (achou)
        return 1;
    else
        return -1;
}

/**
 * @brief Exibe os medicos cadastrados no sistema.
 *
 */
void exibeMedicos()
{
    struct MEDICO medico;
    FILE *lista_medicos;

    if ((lista_medicos = fopen("lista_medicos.txt", "r+b")) != NULL)
    {
        fread(&medico, sizeof(struct MEDICO), 1, lista_medicos);
        while (!feof(lista_medicos))
        {
            printf("\n");
            printf("Codigo: %s\n", medico.codigo);
            printf("Nome: %s\n", medico.nome);
            printf("Telefone: %s\n", medico.telefone);
            printf("Especialidade: %s\n", medico.especialidade);
            printf("\n");

            fread(&medico, sizeof(struct MEDICO), 1, lista_medicos);
        }
    }
    else
        printf("\nErro na abertura do arquivo, a lista de medicos nao pode ser exibida.\n");

    fclose(lista_medicos);
}

// --------------------------------------------------------------------------------
// FUNCIONALIDADES RELACIONADAS A CONSULTAS.
// --------------------------------------------------------------------------------
/**
 * @brief Funcao responsavel pelo cadastro de uma nova consulta.
 *
 */
void cadastrarConsulta()
{
    struct PACIENTE paciente;
    struct MEDICO medico;
    struct CONSULTA consulta;
    FILE *lista_medicos, *lista_pacientes, *lista_consultas;

    int pacienteCadastrado = 0, medicoCadastrado = 0, sequenciaNumerica, cont = 0, limiteAtingido = 0;
    char codigoPaciente[7], codigoMedico[7], codigoString[7], dataSugerida[11], horarioSugerido[6];

    printf("\nInforme o codigo do paciente: ");
    fflush(stdin);
    gets(codigoPaciente);
    printf("\nInforme o codigo do medico: ");
    fflush(stdin);
    gets(codigoMedico);

    // Verifica se existe um paciente com o codigo indicado.
    if ((lista_pacientes = fopen("lista_pacientes.txt", "r+b")) != NULL)
    {
        fread(&paciente, sizeof(struct PACIENTE), 1, lista_pacientes);
        while (!feof(lista_pacientes))
        {
            if (strcmp(paciente.codigo, codigoPaciente) == 0)
            {
                pacienteCadastrado = !pacienteCadastrado;
                strcpy(consulta.nomePaciente, paciente.nome);
            }
            fread(&paciente, sizeof(struct PACIENTE), 1, lista_pacientes);
        }
    }
    fclose(lista_pacientes);

    // Verifica se existe um medico com o codigo indicado.
    if ((lista_medicos = fopen("lista_medicos.txt", "r+b")) != NULL)
    {
        fread(&medico, sizeof(struct MEDICO), 1, lista_medicos);
        while (!feof(lista_medicos))
        {
            if (strcmp(medico.codigo, codigoMedico) == 0)
            {
                medicoCadastrado = !medicoCadastrado;
                strcpy(consulta.nomeMedico, medico.nome);
            }
            fread(&medico, sizeof(struct MEDICO), 1, lista_medicos);
        }
    }
    fclose(lista_medicos);

    if (!medicoCadastrado || !pacienteCadastrado)
        printf("\nMedico ou paciente nao cadastrados no sistema. Verifique as informacoes inseridas.\n");
    else
    {
        if ((lista_consultas = fopen("lista_consultas.txt", "r+b")) == NULL)
        {
            printf("\nArquivo nao existente, gerando arquivo base.\n");

            if ((lista_consultas = fopen("lista_consultas.txt", "w+b")) == NULL)
            {
                printf("\nErro na criacao do arquivo.\n");
                exit(1);
            }
            system("pause");
        }

        // [1] Gera a sequencia numerica do codigo, o converte em string, e em seguida adiciona o digito relativo de usuario 'P'.
        sequenciaNumerica = geraCodigo();
        sprintf(codigoString, "%i", sequenciaNumerica);
        strcat(codigoString, "-C");
        // Verifica se o codigo atribuido a consulta ja foi utilizado. Se sim, refaz a operacao.
        while (disponibilidadeCodigo_Consulta(lista_consultas, codigoString) != -1)
        {
            sequenciaNumerica = geraCodigo();
            // Verificar comentario [1].
            sprintf(codigoString, "%i", sequenciaNumerica);
            strcat(codigoString, "-C");
            cont++;

            // O valor de 9000 e relativo ao numero maximo de combinacoes (de 9.999 a 1.000).
            if (cont == 9000)
            {
                printf("\nLimite de consultas atingido, operacao cancelada. Entre em contato com o administrador sistema para verificar o problema.\n");
                limiteAtingido = !limiteAtingido;
                break;
            }
        }

        // Apenas cadastra uma nova consulta se nao tivermos atingidos o limite de codigos disponiveis.
        if (!limiteAtingido)
        {
            strcpy(consulta.codigo, codigoString);
            strcpy(consulta.codigoMedico, codigoMedico);
            strcpy(consulta.codigoPaciente, codigoPaciente);
            printf("\nCodigo da consulta: %s\n", consulta.codigo);
            printf("\nInforme a data da consulta (formato dd/mm/aaaa): ");
            fflush(stdin);
            gets(dataSugerida);
            printf("\nInforme a hora da consulta (formato hh:mm): ");
            fflush(stdin);
            gets(horarioSugerido);

            if ((confereDisponilidadeDataSugerida(lista_consultas, dataSugerida, codigoMedico) < 2) && (confereDisponilidadeHorarioSugerido(lista_consultas, horarioSugerido, codigoMedico, dataSugerida) == 1))
            {
                // Caso a data e horario sugerido estejam disponiveis, salva a informacao.
                strcpy(consulta.data, dataSugerida);
                strcpy(consulta.hora, horarioSugerido);
                fseek(lista_consultas, 0, SEEK_END);
                fwrite(&consulta, sizeof(struct CONSULTA), 1, lista_consultas);
                fclose(lista_consultas);

                printf("\n*Consulta marcada com sucesso!*\n\n");
            }
            else
                printf("\nHorario ou data solicitados nao disponiveis. Tente novamente em outra data ou horario.\n\n");
        }
    }
}

/**
 * @brief Funcao que verifica se o codigo do indicado ja esta ou nao sendo utilizado por outra consulta.
 *
 * @param lista_consultas
 * @param codigo
 * @return int
 */
int disponibilidadeCodigo_Consulta(FILE *lista_consultas, char codigo[7])
{
    int posicao = 1, achou = 0;

    struct CONSULTA consulta;

    fseek(lista_consultas, 0, SEEK_SET);
    fread(&consulta, sizeof(struct CONSULTA), 1, lista_consultas);

    while (!feof(lista_consultas) && !achou)
    {
        posicao++;

        if (strcmp(consulta.codigo, codigo) == 0)
            achou = 1;

        fread(&consulta, sizeof(struct CONSULTA), 1, lista_consultas);
    }

    if (achou)
        return 1;
    else
        return -1;
}

/**
 * @brief Exibe as consultas cadastradas no sistema.
 *
 */
void exibeConsultas()
{
    struct CONSULTA consulta;
    FILE *lista_consultas;

    if ((lista_consultas = fopen("lista_consultas.txt", "r+b")) != NULL)
    {
        fread(&consulta, sizeof(struct CONSULTA), 1, lista_consultas);
        while (!feof(lista_consultas))
        {
            printf("\n");
            printf("Codigo da consulta: %s\n", consulta.codigo);
            printf("Codigo do medico: %s\n", consulta.codigoMedico);
            printf("Nome do medico: %s\n", consulta.nomeMedico);
            printf("Codigo do paciente: %s\n", consulta.codigoPaciente);
            printf("Nome do paciente: %s\n", consulta.nomePaciente);
            printf("Data: %s\n", consulta.data);
            printf("Hora: %s\n", consulta.hora);
            printf("\n");

            fread(&consulta, sizeof(struct CONSULTA), 1, lista_consultas);
        }
    }
    else
        printf("\nErro na abertura do arquivo, a lista de consultas nao pode ser exibida.\n");

    fclose(lista_consultas);
}

/**
 * @brief Verifica se a data sugerida pelo usuario para a consulta esta disponivel para agendamentos.
 *
 * @param lista_consultas
 * @param dataSugerida
 * @param codigoMedico
 * @return int
 */
int confereDisponilidadeDataSugerida(FILE *lista_consultas, char dataSugerida[11], char codigoMedico[7])
{
    int consultasNaDataSugerida = 0;

    struct CONSULTA consulta;

    fseek(lista_consultas, 0, SEEK_SET);
    fread(&consulta, sizeof(struct CONSULTA), 1, lista_consultas);

    while (!feof(lista_consultas))
    {
        if ((strcmp(consulta.codigoMedico, codigoMedico) == 0) && (strcmp(consulta.data, dataSugerida) == 0))
            consultasNaDataSugerida++;

        fread(&consulta, sizeof(struct CONSULTA), 1, lista_consultas);
    }
    return consultasNaDataSugerida;
}

/**
 * @brief Converte um horario no formato hh:mm em segundos.
 *
 * @param horarioSugerido
 * @return int
 */
int converteHorarioEmSeg(char horarioSugerido[6])
{
    char horaString[3], minutoString[3];
    int horaInt, minutoInt, horarioEmSeg;

    horaString[0] = horarioSugerido[0];
    horaString[1] = horarioSugerido[1];
    minutoString[0] = horarioSugerido[3];
    minutoString[1] = horarioSugerido[4];
    horaInt = atoi(horaString);
    minutoInt = atoi(minutoString);
    horarioEmSeg = horaInt * 3600 + minutoInt * 60;

    return horarioEmSeg;
}

/**
 * @brief Verifica se o horario sugerido pelo usuario para a consulta esta disponivel para agendamentos.
 *
 * @param lista_consultas
 * @param horarioSugerido
 * @param codigoMedico
 * @param dataSugerida
 * @return int
 */
int confereDisponilidadeHorarioSugerido(FILE *lista_consultas, char horarioSugerido[6], char codigoMedico[7], char dataSugerida[11])
{
    int horarioValido = 0, cont = 0;

    struct CONSULTA consulta;

    fseek(lista_consultas, 0, SEEK_SET);
    fread(&consulta, sizeof(struct CONSULTA), 1, lista_consultas);

    while (!feof(lista_consultas))
    {
        // [1] Verifica se existe consulta do medico indicado, na data sugerida. Seguir para comentario [2].
        if ((strcmp(consulta.codigoMedico, codigoMedico) == 0) && (strcmp(consulta.data, dataSugerida) == 0))
        {
            cont++;

            // [2] Se sim, verifica se o tempo entre a consulta ja existente, e a que deseja marcar, e superior a 1.800 segundos (30 minutos).
            if ((abs(converteHorarioEmSeg(horarioSugerido) - converteHorarioEmSeg(consulta.hora)) >= 1800))
                horarioValido = !horarioValido;
        }

        fread(&consulta, sizeof(struct CONSULTA), 1, lista_consultas);
    }

    if (!cont)
        horarioValido = !horarioValido;

    return horarioValido;
}

/**
 * @brief Cancela uma determinada consulta indicada pelo usuario.
 *
 */
void cancelarConsulta()
{
    struct CONSULTA consulta;
    FILE *lista_consultas, *temp____lista_consultas;

    char codigoConsulta[7];

    printf("\nDigite o codigo da consulta que deseja cancelar: ");
    fflush(stdin);
    gets(codigoConsulta);

    // Se a string 'codigoConsulta' nao estiver vazia.
    if (codigoConsulta[0] != '\0')
    {
        if ((lista_consultas = fopen("lista_consultas.txt", "r+b")) != NULL)
        {
            // Apos abrir o arquivo principal, cria um arquivo temporario, que servira como auxiliar.
            if ((temp____lista_consultas = fopen("temp____lista_consultas.txt", "w+b")) != NULL)
            {
                fread(&consulta, sizeof(struct CONSULTA), 1, lista_consultas);
                while (!feof(lista_consultas))
                {
                    // Copiar para o arquivo temporario, todas as consultas armazenadas, exceto a indicada pelo paciente para cancelamento.
                    if (strcmp(consulta.codigo, codigoConsulta) != 0)
                    {
                        fseek(temp____lista_consultas, 0, SEEK_END);
                        fwrite(&consulta, sizeof(struct CONSULTA), 1, temp____lista_consultas);
                    }

                    fread(&consulta, sizeof(struct CONSULTA), 1, lista_consultas);
                }
                printf("\n*Consulta cancelada com sucesso!*\n\n");
            }
            else
                printf("\nErro na abertura do arquivo temporario.\n");
        }
        else
            printf("\nErro na abertura do arquivo, a lista de consultas nao pode ser exibida.\n");

        fclose(lista_consultas);
        fclose(temp____lista_consultas);
        // Apos preencher o arquivo temporario com as consultas, o arquivo 'lista_consultas' e deletado.
        // O arquivo temporario 'temp____lista_consultas' e renomeado para 'lista_consultas'.
        remove("lista_consultas.txt");
        rename("temp____lista_consultas.txt", "lista_consultas.txt");
    }
    else
        printf("\nConsulta nao localizada no nosso sistema.\n");
}

// --------------------------------------------------------------------------------
// EMISSAO DE RELATORIOS.
// --------------------------------------------------------------------------------
/**
 * @brief Exibe as consultas marcadas para uma data especifica (dd/mm/aaaa).
 *
 */
void exibeConsultasNaData()
{
    struct CONSULTA consulta;
    FILE *lista_consultas;

    char data[11];

    printf("\nDigite a data em que deseja verificar as consultas: ");
    fflush(stdin);
    gets(data);

    if ((lista_consultas = fopen("lista_consultas.txt", "r+b")) != NULL)
    {
        fread(&consulta, sizeof(struct CONSULTA), 1, lista_consultas);
        while (!feof(lista_consultas))
        {
            if (strcmp(consulta.data, data) == 0)
            {
                printf("\n");
                printf("Codigo da consulta: %s\n", consulta.codigo);
                printf("Codigo do medico: %s\n", consulta.codigoMedico);
                printf("Nome do medico: %s\n", consulta.nomeMedico);
                printf("Codigo do paciente: %s\n", consulta.codigoPaciente);
                printf("Nome do paciente: %s\n", consulta.nomePaciente);
                printf("Data: %s\n", consulta.data);
                printf("Hora: %s\n", consulta.hora);
                printf("\n");
            }
            fread(&consulta, sizeof(struct CONSULTA), 1, lista_consultas);
        }
    }
    else
        printf("\nErro na abertura do arquivo, a lista de consultas nao pode ser exibida.\n");

    fclose(lista_consultas);
}

/**
 * @brief Exibe as consultas realizadas por um determinado paciente ate a data corrente (dd/mm/aaaa).
 *
 */
void exibeConsultasRealizadasAteDataCorrente()
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    struct CONSULTA consulta;
    FILE *lista_consultas;

    char localizador[50];

    printf("\nDigite o nome ou codigo do paciente que deseja verificar: ");
    fflush(stdin);
    gets(localizador);

    if ((lista_consultas = fopen("lista_consultas.txt", "r+b")) != NULL)
    {
        fread(&consulta, sizeof(struct CONSULTA), 1, lista_consultas);
        while (!feof(lista_consultas))
        {
            if ((strcmp(consulta.codigoPaciente, localizador) == 0) || (strcmp(consulta.nomePaciente, localizador) == 0))
            {
                if (converteDataEmInt(consulta.data) < (tm.tm_year + 1900) * 10000 + tm.tm_mon * 100 + tm.tm_mday)
                {
                    printf("\n");
                    printf("Codigo da consulta: %s\n", consulta.codigo);
                    printf("Codigo do medico: %s\n", consulta.codigoMedico);
                    printf("Nome do medico: %s\n", consulta.nomeMedico);
                    printf("Codigo do paciente: %s\n", consulta.codigoPaciente);
                    printf("Nome do paciente: %s\n", consulta.nomePaciente);
                    printf("Data: %s\n", consulta.data);
                    printf("Hora: %s\n", consulta.hora);
                    printf("\n");
                }
            }
            fread(&consulta, sizeof(struct CONSULTA), 1, lista_consultas);
        }
    }
    else
        printf("\nErro na abertura do arquivo, a lista de consultas nao pode ser exibida.\n");

    fclose(lista_consultas);
}

/**
 * @brief Converte data em formato dd/mm/aaaa para valor numerico no formato aaaammdd.
 *
 * @param dataString
 * @return int
 */
int converteDataEmInt(char dataString[11])
{
    char diaString[3], mesString[3], anoString[5];
    int dia, mes, ano, inicioString, comprimento, i;

    // Decomposicao do dia.
    inicioString = 0;
    comprimento = 2;
    i = 0;
    while (inicioString < comprimento)
    {
        diaString[i] = dataString[inicioString];
        inicioString++;
        i++;
    }
    dia = atoi(diaString);

    // Decomposicao do mes.
    inicioString = 3;
    comprimento = 5;
    i = 0;
    while (inicioString < comprimento)
    {
        mesString[i] = dataString[inicioString];
        inicioString++;
        i++;
    }
    mes = atoi(mesString);

    // Decomposicao do ano.
    inicioString = 6;
    comprimento = 10;
    i = 0;
    while (inicioString < comprimento)
    {
        anoString[i] = dataString[inicioString];
        inicioString++;
        i++;
    }
    ano = atoi(anoString);

    return ano * 10000 + mes * 100 + dia;
}

/**
 * @brief Exibe todas as consultas de um medico especifico.
 *
 */
void exibeConsultasMedicos()
{
    struct CONSULTA consulta;
    FILE *lista_consultas;

    char localizador[50];

    printf("\nDigite o nome ou codigo do medico que deseja verificar: ");
    fflush(stdin);
    gets(localizador);

    if ((lista_consultas = fopen("lista_consultas.txt", "r+b")) != NULL)
    {
        fread(&consulta, sizeof(struct CONSULTA), 1, lista_consultas);
        while (!feof(lista_consultas))
        {
            if ((strcmp(consulta.codigoMedico, localizador) == 0) || (strcmp(consulta.nomeMedico, localizador) == 0))
            {
                printf("\n");
                printf("Codigo da consulta: %s\n", consulta.codigo);
                printf("Codigo do medico: %s\n", consulta.codigoMedico);
                printf("Nome do medico: %s\n", consulta.nomeMedico);
                printf("Codigo do paciente: %s\n", consulta.codigoPaciente);
                printf("Nome do paciente: %s\n", consulta.nomePaciente);
                printf("Data: %s\n", consulta.data);
                printf("Hora: %s\n", consulta.hora);
                printf("\n");
            }
            fread(&consulta, sizeof(struct CONSULTA), 1, lista_consultas);
        }
    }
    else
        printf("\nErro na abertura do arquivo, a lista de consultas nao pode ser exibida.\n");

    fclose(lista_consultas);
}
