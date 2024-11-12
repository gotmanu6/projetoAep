#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#ifdef _WIN32
    #define CLEAR "cls"
#else
    #define CLEAR "clear"
#endif


char* Criptografia(const char *senha) {
    static char senhacriptografada[17];
    const char alfabeto[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    const char substituto[] = "@#$!|%&*()+-_><,.?/~^}{[]";

    for (int i = 0; senha[i] != '\0'; i++) {
        char *pos = strchr(alfabeto, senha[i]);
        senhacriptografada[i] = pos ? substituto[pos - alfabeto] : senha[i];
    }
    senhacriptografada[strlen(senha)] = '\0';
    return senhacriptografada;
}
//-=-=-=-=-=--=-=-=-=-=-=--=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-

int validarRA(const char *ra) {
    if (strlen(ra) != 10) return 0;
    for (int i = 0; i < 8; i++) {
        if (!isdigit(ra[i])) return 0;  
    }
    return ra[8] == '-' && isdigit(ra[9]);  
}

//-=-=-=-=-=--=-=-=-=-=-=--=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-
int verificarSenhaForte(const char *senha) {
    int temLetra = 0, temNumero = 0, temEspecial = 0;
    for (int i = 0; senha[i] != '\0'; i++) {
        if (isalpha(senha[i])) temLetra = 1;
        else if (isdigit(senha[i])) temNumero = 1;
        else temEspecial = 1;
    }
    return temLetra && temNumero && temEspecial && strlen(senha) >= 7 && strlen(senha) <= 16;
}
//-=-=-=-=-=--=-=-=-=-=-=--=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-

void limparTela() {
    system(CLEAR);
    printf("-=-=-= BEM VINDO =-=-=-\n");
    printf("     Sistema da AEP\n");
    printf("Opções:\n 1. Incluir Cadastro.\n 2. Excluir Cadastro.\n 3. Listar Todos os Cadastros.\n 4. Créditos.\n 5. Sair.\n\n");
}
//-=-=-=-=-=--=-=-=-=-=-=--=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-

void aguardarUsuario() {
    printf("\nPressione Enter para voltar ao menu principal...");
    while (getchar() != '\n');  
}
//-=-=-=-=-=--=-=-=-=-=-=--=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-

void salvarCadastro(const char *ra, const char *senhaCripto) {
    FILE *arquivoUsuario = fopen("Usuario.txt", "a");
    FILE *arquivoSenha = fopen("Senha.txt", "a");

    if (arquivoUsuario && arquivoSenha) {
        fprintf(arquivoUsuario, "%s\n", ra);
        fprintf(arquivoSenha, "%s\n", senhaCripto);
        fclose(arquivoUsuario);
        fclose(arquivoSenha);
    } else {
        printf("Erro ao abrir os arquivos!\n");
    }
}

//-=-=-=-=-=--=-=-=-=-=-=--=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-

void listarCadastros() {
    FILE *arquivoUsuario = fopen("Usuario.txt", "r");
    FILE *arquivoSenha = fopen("Senha.txt", "r");

    if (arquivoUsuario && arquivoSenha) {
        char ra[11], senhaCripto[17];
        printf("-=-=-= LISTA =-=-=-\nLista Atualizada\n");
        while (fscanf(arquivoUsuario, "%10s", ra) != EOF && fscanf(arquivoSenha, "%16s", senhaCripto) != EOF) {
            printf("RA: %s, Senha Criptografada: %s\n", ra, senhaCripto);
        }
        fclose(arquivoUsuario);
        fclose(arquivoSenha);
    } else {
        printf("Erro ao abrir os arquivos!\n");
    }
    aguardarUsuario();
}

//-=-=-=-=-=--=-=-=-=-=-=--=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-
void excluirCadastro(const char *ra, const char *senha) {
    FILE *arquivoUsuario = fopen("Usuario.txt", "r");
    FILE *arquivoSenha = fopen("Senha.txt", "r");
    FILE *tempUsuario = fopen("TempUsuario.txt", "w");
    FILE *tempSenha = fopen("TempSenha.txt", "w");

    if (arquivoUsuario && arquivoSenha && tempUsuario && tempSenha) {
        char raLido[11], senhaLida[17];
        char *senhaCripto = Criptografia(senha);
        int encontrado = 0;

        while (fscanf(arquivoUsuario, "%10s", raLido) != EOF && fscanf(arquivoSenha, "%16s", senhaLida) != EOF) {
            if (strcmp(ra, raLido) || strcmp(senhaCripto, senhaLida)) {
                fprintf(tempUsuario, "%s\n", raLido);
                fprintf(tempSenha, "%s\n", senhaLida);
            } else {
                encontrado = 1;
            }
        }

        fclose(arquivoUsuario);
        fclose(arquivoSenha);
        fclose(tempUsuario);
        fclose(tempSenha);

        remove("Usuario.txt");
        remove("Senha.txt");
        rename("TempUsuario.txt", "Usuario.txt");
        rename("TempSenha.txt", "Senha.txt");

        printf(encontrado ? "\nLogin excluído com sucesso!\n" : "\nCadastro não encontrado ou senha incorreta!\n");
    } else {
        printf("Erro ao abrir os arquivos!\n");
    }
    aguardarUsuario();
}
//-=-=-=-=-=--=-=-=-=-=-=--=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-

int main() {
    setlocale(LC_ALL, "Portuguese");
    int op;
    char ra[11], senha[17];

    while (1) {
        limparTela();
        printf("Opção Escolhida: ");
        scanf("%d", &op);
        getchar();  /

        switch(op) {
            case 1:
            	    limparTela();
                printf("-=-=-= INCLUIR CADASTRO =-=-=-\n");
                do {
                    printf("RA (formato XXXXXXXX-X): ");
                    scanf("%10s", ra);
                    getchar();  /
                    if (!validarRA(ra)) printf("Formato de RA inválido! Tente novamente.\n");
                } while (!validarRA(ra));

                do {
                    printf("Senha (entre 7 e 16 caracteres): ");
                    scanf("%16s", senha);
                    getchar();  
                    if (!verificarSenhaForte(senha))
                        printf("Senha inválida! A senha deve ter entre 7 e 16 caracteres, letras, números e caracteres especiais.\n");
                    else
                        break;
                } while (1);

                printf("\nLogin incluído com sucesso!\n");
                salvarCadastro(ra, Criptografia(senha));
                aguardarUsuario();
                break;

            case 2:
            	    limparTela();
                printf("-=-=-= EXCLUIR CADASTRO =-=-=-\n");
                do {
                    printf("RA (formato XXXXXXXX-X): ");
                    scanf("%10s", ra);
                    getchar();  
                    if (!validarRA(ra)) printf("Formato de RA inválido! Tente novamente.\n");
                } while (!validarRA(ra));

                printf("Senha: ");
                scanf("%16s", senha);
                getchar();  
                excluirCadastro(ra, senha);
                break;

            case 3:
            	    limparTela();
                listarCadastros();
                break;

            case 4:
            	    limparTela();
                printf("-=-=-=-=-=-=-= CRÉDITOS =-=-=-=-=-=-=-\nTrabalho feito por:\n");
                printf("Matheus Goulart de Alencar\nThiago Teodoro da Conceição\nEmanuelly Prestes Lopes\n");
                printf("Trabalho da AEP\n-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
                aguardarUsuario();
                break;

            case 5:
                limparTela();
                printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
                printf("Muito obrigado por compilar nosso programa!!\nAté uma próxima!\n");
                printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
                return 0;

            default:
                printf("Opção inválida! Tente novamente.\n");
                aguardarUsuario();
        }
    }
}

