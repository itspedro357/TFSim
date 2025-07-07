#include <vector>

class blp {
public:
    blp(unsigned int history_bits, unsigned int counter_bits);
    bool predict(unsigned int pc);
    void update(unsigned int pc, bool taken, bool hit);
    float get_hit_rate() const;

private:
    unsigned int history_bits;
    unsigned int counter_bits;
    unsigned int max_counter;
    unsigned int msb_mask;
    unsigned int history_mask;

    std::vector<unsigned int> history_table;
    std::vector<unsigned int> prediction_table;

    int total_predictions;
    int correct_predictions;
};