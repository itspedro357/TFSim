#include "blp.hpp"
#include <iostream>

blp::blp(unsigned int history_bits, unsigned int counter_bits)
    : history_bits(history_bits),
      counter_bits(counter_bits),
      total_predictions(0),
      correct_predictions(0)
{
    max_counter = (1 << counter_bits) - 1;
    msb_mask = 1 << (counter_bits - 1);
    history_mask = (1 << history_bits) - 1;

    // Inicializa a tabela de histórico com zeros
    history_table.resize(1 << history_bits, 0);
    // Inicializa a tabela de preditores com valores intermediários
    prediction_table.resize(1 << history_bits, max_counter / 2);
}

bool blp::predict(unsigned int pc)
{
    total_predictions++;
    unsigned int index = (pc >> 2) & history_mask;
    unsigned int history = history_table[index];
    unsigned int counter = prediction_table[history];
    bool prediction = counter >= (max_counter / 2); // Prever "taken" se counter >= max_counter/2
    std::cout << "BLP Predict: PC=" << pc << ", Index=" << index 
              << ", History=" << history << ", Counter=" << counter 
              << ", Prediction=" << (prediction ? "taken" : "not taken") << std::endl;
    return prediction;
}

void blp::update(unsigned int pc, bool taken, bool hit)
{
    unsigned int index = (pc >> 2) & history_mask;
    unsigned int &counter = prediction_table[history_table[index]];
    std::cout << "BLP Update: PC=" << pc << ", Index=" << index 
              << ", History=" << history_table[index] << ", Counter=" << counter 
              << ", Taken=" << taken << ", Hit=" << hit << std::endl;

    if (hit)
        correct_predictions++;

    // Atualiza contador saturado
    if (taken)
    {
        if (counter < max_counter)
            counter++;
        else
            counter = max_counter; // Garante limite superior
    }
    else
    {
        if (counter > 0)
            counter--;
        else
            counter = 0; // Garante limite inferior
    }

    // Atualiza histórico
    history_table[index] = ((history_table[index] << 1) | (taken ? 1 : 0)) & history_mask;
    std::cout << "BLP Updated: New History=" << history_table[index] << ", New Counter=" << counter << std::endl;
}

float blp::get_hit_rate() const
{
    if (total_predictions == 0)
        return 0.0f;
    return (float)correct_predictions / total_predictions * 100.0f;
}