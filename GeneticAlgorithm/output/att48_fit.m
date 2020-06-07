a = importdata('att48_roulette.f');
b = a.data;

generation = b(:, 1);
fitness = b(:, 2);
avgfitness = b(:, 3);

figure(1)
plot(generation, fitness, 'r', generation, avgfitness, 'b')
xlim([0 max(generation)])
legend('Fitness of best solution','Average population fitness')
saveas(1, 'plots/att48_fit_roulette.png')
figure(2)
loglog(generation, fitness, 'r', generation, avgfitness, 'b')
xlim([0 max(generation)])
legend('Fitness of best solution','Average population fitness')
saveas(1, 'plots/att48_fit_log_roulette.png')
