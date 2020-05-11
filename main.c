#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Vérifier si l'état présent est un état acceptant.
int is_state_accepted(char *state, char *accepting_states)
{
    for (int i = 0; i < sizeof(accepting_states) / sizeof(accepting_states[0]); i++)
        if (accepting_states[i] == *state)
            return 1;
    return 0;
}

// Trouver tous les états vers lesquels une transition est possible.
void get_states(char delim, char *buf, char character, int states[])
{
    size_t i = 0;
    int state = 0;
    int states_count = 0;

    while (buf[i] != '\0')
    {
        if (buf[i] == delim)
            state++;
        else if (buf[i] == character)
            states[states_count++] = state;
        i++;
    }
}

int test_word(char alphabet[], int state, char accepting_states[], int states_count, char transitions[states_count][states_count], char *word)
{
    // Si le mot ne contient pas de lettres, on vérifie si l'état présent est un état acceptant.
    if (strlen(word) == 0)
    {
        char state_str[256];
        sprintf(state_str, "%d", state);
        return is_state_accepted(state_str, accepting_states);
    }

    // Vérifier que la première lettre est bien dans l'alphabet de l'automate.
    if (strchr(alphabet, word[0]) == NULL)
        return 0;

    int new_states[5] = {[0 ... 4] = -1};

    get_states(';', transitions[state], word[0], new_states);

    // Enlever la première lettre du mot, puisque on a déja vérifié qu'elle est compatible avec l'automate.
    memmove(&word[0], &word[1], strlen(word));

    // Pour chaque transition possible, on teste récursivement les lettres suivantes.
    for (size_t i = 0; i < 5; i++)
    {
        if (new_states[i] == -1)
            break;

        // Si toutes les lettres qui suivent conviennent à la définition de l'automate, pas besoin de continuer, on a bien vérifié que le mot est compatible.
        if (test_word(alphabet, new_states[i], accepting_states, states_count, transitions, word) == 1)
            return 1;
    }

    // Le mot n'est pazs compatible avec l'automate.
    return 0;
}

int main()
{
    char file_name[256];
    char mot[256];

    printf("Nom du fichier décrivant l'automate:\n");
    scanf("%s", file_name);
    printf("Mot:\n");
    scanf("%s", mot);

    // Création du stream pour lire le fichier
    FILE *fp;
    fp = fopen(file_name, "r");
    if (fp == NULL)
    {
        perror("Erreur d'ouverture du fichier");
        return -1;
    }

    char line[60];
    char alphabet[256];
    int state, states_count;
    char accepting_states[256];

    // 1ère ligne: Alphabet
    fgets(line, 60, fp);
    strcpy(alphabet, line);

    // 2ème ligne: État Initial
    fgets(line, 60, fp);
    state = atoi(line);

    // 3ème ligne: États Acceptants
    fgets(line, 60, fp);
    strcpy(accepting_states, line);

    // 4ème ligne: Nombre d'États
    fgets(line, 60, fp);
    states_count = atoi(line);

    // 5ème ligne et plus: Fonctions de Transition
    char transitions[states_count][states_count];
    for (int i = 0; i < states_count; i++)
    {
        fgets(line, 60, fp);
        strcpy(transitions[i], line);
    }

    // Fermer le stream
    fclose(fp);

    // Sortie
    printf("%s => ", mot);
    printf("%s\n", test_word(alphabet, state, accepting_states, states_count, transitions, mot) == 0 ? "NON" : "OUI");

    return 0;
}
