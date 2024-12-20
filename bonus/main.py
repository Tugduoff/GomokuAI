##
## EPITECH PROJECT, 2024
## GomokuAI
## File description:
## genetic
##

import random
import subprocess

def generate_individual():
    return {
        "D4_pattern_ai": random.randint(0, 1000000),
        "S4_pattern_ai": random.randint(0, 1000000),
        "D3_pattern_ai": random.randint(0, 1000000),
        "S3_pattern_ai": random.randint(0, 1000000),
        "D2_pattern_ai": random.randint(0, 1000000),
        "S2_pattern_ai": random.randint(0, 1000000),

        "D4_pattern_pl": random.randint(-1000000, 0),
        "S4_pattern_pl": random.randint(-1000000, 0),
        "D3_pattern_pl": random.randint(-1000000, 0),
        "S3_pattern_pl": random.randint(-1000000, 0),
        "D2_pattern_pl": random.randint(-1000000, 0),
        "S2_pattern_pl": random.randint(-1000000, 0),

        "timeStop": random.randint(1, 4),
        "max_depth": random.randint(1, 6),
    }

def evaluate_individual(individual):
    params = [
        str(individual['D4_pattern_ai']), str(individual['S4_pattern_ai']),
        str(individual['D3_pattern_ai']), str(individual['S3_pattern_ai']),
        str(individual['D2_pattern_ai']), str(individual['S2_pattern_ai']),
        str(individual['D4_pattern_pl']), str(individual['S4_pattern_pl']),
        str(individual['D3_pattern_pl']), str(individual['S3_pattern_pl']),
        str(individual['D2_pattern_pl']), str(individual['S2_pattern_pl']),
        str(individual['timeStop']), str(individual['max_depth'])
    ]
    with open('../tests/tt', 'r') as input_file:
        result = subprocess.run(["./pbrain-gomoku-ai"] + params, stdin=input_file, capture_output=True, text=True)
    print(result.returncode)
    return result.returncode

def crossover(parent1, parent2):
    child = {}
    for key in parent1:
        child[key] = random.choice([parent1[key], parent2[key]])
    return child

def mutate(individual):
    if random.random() < 0.2:
        param = random.choice(list(individual.keys()))
        if param == "timeStop":
            individual[param] = random.randint(1, 4)
        elif param == "max_depth":
            individual[param] = random.randint(1, 6)
        elif param.endswith("_pattern_ai"):
            individual[param] = random.randint(0, 1000000)
        elif param.endswith("_pattern_pl"):
            individual[param] = random.randint(-1000000, 0)

def genetic_algorithm(generations=20, population_size=10):
    population = [generate_individual() for _ in range(population_size)]

    for generation in range(generations):
        scores = [(ind, evaluate_individual(ind)) for ind in population]
        scores.sort(key=lambda x: x[1])
        population = [ind for ind, _ in scores[:population_size // 2]]
        new_population = []

        while len(new_population) < population_size:
            parent1, parent2 = random.sample(population, 2)
            child = crossover(parent1, parent2)
            mutate(child)
            new_population.append(child)

        population = new_population
        print(f"Génération {generation} - Meilleur score : {scores[0][1]}")
    return scores[0][0] # À changer (voir le méthode pour return les data)

if __name__ == "__main__":
    best_params = genetic_algorithm()
    print("Meilleurs paramètres trouvés :", best_params)
    exit(0)
