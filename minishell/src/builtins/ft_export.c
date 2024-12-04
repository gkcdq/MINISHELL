#include "../../minishell.h"

int find_env_variable(char **envp, const char *key) 
{
    int i = 0;
    size_t key_len = strlen(key);

    while (envp[i]) {
        if (strncmp(envp[i], key, key_len) == 0 && envp[i][key_len] == '=') {
            return i; // Index de la variable trouvée
        }
        i++;
    }
    return -1; // Non trouvé
}


//a changer v
void export_variable(t_ee *ee, const char *input) 
{
    char *key = strtok(strdup(input), "="); // Récupérer la clé (avant le "=")
    int index = find_env_variable(ee->envp, key);

    if (index != -1) 
    {
        // Si la variable existe, mettez-la à jour
        free(ee->envp[index]);
        ee->envp[index] = strdup(input);
    } else {
        // Sinon, ajoutez une nouvelle variable
        int len = 0;
        while (ee->envp[len]) len++;

        char **new_envp = malloc((len + 2) * sizeof(char *));
        int i = 0;
        while (i < len) {
            new_envp[i] = ee->envp[i];
            i++;
        }
        new_envp[len] = strdup(input);
        new_envp[len + 1] = NULL;

        free(ee->envp);
        ee->envp = new_envp;
    }

    free(key); // Libérez la clé temporaire
}

// Fonction principale export
void ft_export(char *input, t_ee *ee) 
{
    char **args;
    char **sorted_env;
    int i_outer;
    int i_inner;
    char *tmp;

    args = ft_split(input, ' ');
    if (args[1] == NULL) 
    {
        int len = 0;
        while (ee->envp[len])
            len++;
        sorted_env = malloc(sizeof(char *) * (len + 1));
        int i = 0;
        while (i < len)
        {
            sorted_env[i] = ee->envp[i];
            i++;
        }
        sorted_env[len] = NULL;
        i_outer = 0;
        while (i_outer < len - 1) 
        {
            i_inner = i_outer + 1;
            while (i_inner < len) {
                if (ft_strcmp(sorted_env[i_outer], sorted_env[i_inner]) > 0) {
                    tmp = sorted_env[i_outer];
                    sorted_env[i_outer] = sorted_env[i_inner];
                    sorted_env[i_inner] = tmp;
                }
                i_inner++;
            }
            i_outer++;
        }
        int j = 0;
        while (j < len) 
        {
            printf("⚙️_(o_o;)_⚙️  %s\n", sorted_env[j]);
            j++;
        }
        free(sorted_env);
    } 
    else 
        export_variable(ee, input);
    free_split(args);
}
