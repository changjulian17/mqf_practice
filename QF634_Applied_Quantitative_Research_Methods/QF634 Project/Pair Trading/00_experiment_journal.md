### 30 Nov

- first few tries, network is very large
- added epsilon search in "choose_action" functionso that there will be some chance to explore
- changed reward function to multiply losses and give exponential returns to incentivise risk taking

### 1 dec 2105:

- might have performance is always oscillating negative and positive. This might be because of too large a learning rate. also start from start of training periods max steps to be 3000 so that total results are comparable
  - this helped quite abit.

`input_size = 7  # Adjust to your specific input size output_size = 3  # Adjust to your desired number of discrete actions learning_rate = 0.1 discount_factor = 0.8 epsilon = 1 # down to .3 epsilon_decay = 0.9999 num_episodes = 500 max_steps_per_episode = 3000`

- want to try changing epsilon to only update after the entire episode instead of after each step. its decaying too quickly
- I want to try with changing reward by changing "learn" to use total_reward instead of "reward"
- Scale the states. need to explore scaling the state since it is still in terms of absolute differences. NN is not able to do proportions
- training epochs should be smaller at up to 30 days because mean reversion pattern is 1 to 33 days
  - very bad performance with 40 day epochs

### 1 dec 2217:

- changed target q value fxn to remove exponential reward and scaled negative reward. now both positive and negative are the same. added portion of total reward in episode to incentivise more long term rewards.
  - `        if reward > 0: target_q_value = reward + self.discount_factor * next_q_value * (1 - done) + total_reward * .1 else: target_q_value = reward + self.discount_factor * next_q_value * (1 - done) + total_reward * .1`
  - `  if episode%1==0: agent.epsilon *= agent.epsilon_decay`

### 2 Dec 2101:- managed to scale but results are not any better

- thinking of reducing learning rate to reduce the oscillations
  - will try to run with learning rate at 0.01
- right now total reward is taking all of the target q function. maybe can make it a 50/50 split


### 3 dec

- training taking
  - a full length training dataset.
  - 1000 per learning step
  - learning rate test (1.5 mins per episode)
    - episodes: 5
    - learning rate: 0.05 ==> total reward: .037
    - learning rate: 0.5 ==> total reward: -.6
    - learning rate: 0.3 ==> total reward: 0.88, .044
    - learning rate: 0.15 ==> total reward: -.023
    - learning rate: 0.25 ==> total reward: -.6
    - learning rate: 0.35 ==> total reward: .028
  - with drop out layer test (1.75 mins per episode)
    - learning rate: 0.3 ==> total reward: -0.488
    - learning rate: 0.4 ==> total reward: -0.422
    - learning rate: 0.5 ==> total reward: .26, .13, .096
    - learning rate: 0.6 ==> total reward: .03
    - learning rate: 0.7 ==> total reward: -.08
- performance still bad. should include dropout layer? --> performance a bit worse but more consistent
- try removing spread so that the input is only boolean of SD and last position
  - drop out layer test (1.75 mins per episode)
    - learning rate: 0.1 ==> total reward: -.36
    - learning rate: 0.3 ==> total reward: -.01
    - learning rate: 0.5 ==> total reward: -.26

### 5 dec
- previously target and online network updated at the same time, but it should be used to regularise. So will try with updating more periodically instead of every learn step. reduced NN to 16 and 8 hidden layers with dropout (1.67 min per episode)
    - learning rate: 0.3, update target every 10 learn occurences ==> total reward: .03
    - learning rate: 0.3, update target every 100 learn occurences ==> total reward: .26
    - learning rate: 0.3, update target every 500 learn occurences ==> total reward: -.383
    - learning rate: 0.3, update target every 600 learn occurences ==> total reward: .22
    - learning rate: 0.3, update target every 675 learn occurences ==> total reward: .41
    - learning rate: 0.3, update target every 700 learn occurences ==> total reward: .10, .03
    - learning rate: 0.3, update target every 750 learn occurences ==> total reward: .40, .15, -0.353
    - learning rate: 0.3, update target every 1000 learn occurences ==> total reward: .05
    - learning rate: 0.3, update target every 10 learn occurences, remove dropout layers ==> total reward: -.203
    - learning rate: 0.5, update target every 10 learn occurences, remove dropout layers ==> total reward: -.334
- changed ADAM optimiser to SGD (1.6 min per episode)
    - learning rate: 0.15, update target every 750 learn occurences ==> total reward: .084
    - learning rate: 0.3, update target every 750 learn occurences ==> total reward: -.20
    - learning rate: 0.5, update target every 750 learn occurences ==> total reward: .303
    - learning rate: 0.6, update target every 750 learn occurences ==> total reward: -.110
    - learning rate: 0.7, update target every 750 learn occurences ==> total reward: -.556, -0.449
- long term run with 50 episodes
    - SGD, learning rate: 0.5, update target every 750 learn occurences ==> total reward: -.04 (79 min)
    - ADAM, learning rate: 0.3, update target every 750 learn occurences ==> total reward: -.01 (84.5 min)
    - ADAM, 32 hidden layer, learning rate: 0.3, update target every 750 learn occurences ==> total reward: 0.54 (88 min)
- long term run with 300 episodes
    - ADAM, 32X16 hidden layer, learning rate: 0.3, update target every 750 learn occurences ==> total reward:  -0.28(540 min)

### 6 Dec

1. reduce to only 4 SD flags - Done
2. discount factor up to .99 - Done
3. try increasing punishment with X10 negative reward if less than 0 - Done

- there is a positive gradient
  - 5 episodes, X10 negative reward ADAM, 32X16 hidden layer, learning rate: 0.3, update target every 750 learn occurences ==> total reward: .05
  - 5 episodes, X10 negative reward ADAM, 32X8 hidden layer, learning rate: 0.3, update target every 750 learn occurences ==> total reward: -0.01
  - 5 episodes, X100 negative reward ADAM, 32X16 hidden layer, learning rate: 0.3, update target every 750 learn occurences ==> total reward: -0.01 (8 mins)
  - 5 episodes, X100 negative reward ADAM, 32X16 hidden layer, learning rate: 0.3, update target every 750 learn occurences ==> total reward: 0.014
  - 300 episodes, X100 negative reward ADAM, 32X16 hidden layer, learning rate: 0.3, update target every 750 learn occurences ==> total reward: 0.017
  - 50 episodes, X100 negative reward ADAM, 32X16 hidden layer, learning rate: 0.3, update target every 750 learn occurences, discount_factor = 0.99 ==> total reward:
- try reducing net to only 4 neurons
  - 5 episodes, X100 negative reward ADAM, 4X4 hidden layer, learning rate: 0.3, update target every 750 learn occurences ==> total reward: 0.0
  - 10 episodes, X100 negative reward ADAM, 4X4 hidden layer, learning rate: 0.3, update target every 750 learn occurences ==> total reward: 0.0

### 8 Dec
- 50 episodes, X100 negative reward ADAM, 4X4 hidden layer, learning rate: 0.3, update target every 750 learn occurences, discount_factor = 0.99 ==> total reward: 0.118
- 300 episodes, X100 negative reward ADAM, 4X4 hidden layer, learning rate: 0.3, update target every 750 learn occurences, discount_factor = 0.99 ==> total reward: -0.02 (epsilon 10), 0.29 (epsilon .46)
- try removing previous action in state space to move to 4 state spaces only . 4X4
  - 4X4, 10 episodes, X100 negative reward ADAM, 4X4 hidden layer, learning rate: 0.3, update target every 750 learn occurences, discount_factor = 0.99 ==> total reward: 0.282
  - 4X4, epsilon_decay = 0.955, 20 episodes, X100 negative reward ADAM, learning rate: 0.3, update target every 750 learn occurences, discount_factor = 0.99 ==> total reward: -.20
  - 4X4, epsilon_decay = 0.9885, 20 episodes, X100 negative reward ADAM, learning rate: 0.3, update target every 750 learn occurences, discount_factor = 0.99 ==> total reward: 0.08
  - 32X4, epsilon_decay = 0.9885, 20 episodes, X100 negative reward ADAM, learning rate: 0.3, update target every 750 learn occurences, discount_factor = 0.99 ==> total reward: 0.07
  - add back dropout layers, 32X4, epsilon_decay = 0.9885, 20 episodes, X100 negative reward ADAM, learning rate: 0.3, update target every 750 learn occurences, discount_factor = 0.99 ==> total reward: .21
  - add back dropout layers, 32X4, epsilon_decay = 0.9885, 10 episodes, X100 negative reward ADAM, learning rate: 0.3, update target every 750 learn occurences, discount_factor = 0.5 ==> total reward: .002
  - add back dropout layers, 32X4, epsilon_decay = 0.9885, 10 episodes, X100 negative reward ADAM, learning rate: 0.3, update target every 750 learn occurences, discount_factor = 0.5 ==> total reward: .002
  - add back dropout layers, 32X4, epsilon_decay = 0.9885, 300 episodes, X100 negative reward ADAM, learning rate: 0.3, update target every 750 learn occurences, discount_factor = 0.99 ==> total reward: 0.115

### 9 Dec

- state cases should be imbalanced
  - add back dropout layers, 32X4, epsilon_decay = 0.9885, 20 episodes,  ADAM, learning rate: 0.3, update target every 1000 learn occurences, discount_factor = 0.99 ==> total reward: -0.29
  - add back dropout layers, 32X4, epsilon_decay = 0.9885, 5 episodes,  ADAM, learning rate: 0.3, update target every 1000 learn occurences, discount_factor = 0.99 ==> total reward: 0.223
  - add back dropout layers, 32X4, epsilon_decay = 0.9885, 5 episodes, ADAM, learning rate: 0.3, update target every 550 learn occurences, discount_factor = 0.99 ==> total reward: 0.126
  - add back dropout layers, 32X4, epsilon_decay = 0.9885, 10 episodes,  ADAM, learning rate: 0.3, update target every 550 learn occurences, discount_factor = 0.99 ==> total reward:
- changed data set to 17 pairs
  - add back dropout layers, 32X4, epsilon_decay = 0.9885, 10 episodes,  ADAM, learning rate: 0.3, update target every 550 learn occurences, discount_factor = 0.99 ==> total reward: -0.16
  - 17 pairs, add back dropout layers, 32X4, epsilon_decay = 0.9885, 5 episodes,  ADAM, learning rate: 0.3, update target every 550 learn occurences, discount_factor = 0.99, changed reward *5 for positive, -1 for negative ==> total reward:0.06 (at least got positive gradient), try run again to get more data on consistency. also need to observe if exploiting might be necessary
  - 17 pairs, add back dropout layers, 32X4, epsilon_decay = 0.9885, 5 episodes,  ADAM, learning rate: 0.3, update target every 550 learn occurences, discount_factor = 0.99, changed reward *5 for positive, -1 for negative ==> total reward:0.087

### 10 Dec

- changed data set to 17 pairs
  - 17 pairs, add back dropout layers, 32X4, epsilon_decay = 0.9885, 10 episodes,  ADAM, learning rate: 0.3, update target every 550 learn occurences, discount_factor = 0.99, changed reward *5 for positive, -1 for negative ==> total reward:-0.10
- no need for complicated network, reduce to 8 (5 min per ep)
  - 17 pairs, add back dropout layers, 8X4, epsilon_decay = 0.9885, 5 episodes,  ADAM, learning rate: 0.3, update target every 550 learn occurences, discount_factor = 0.99, changed reward *5 for positive, -1 for negative ==> total reward: -0.113
- try removing dropout layers
  - 17 pairs, add back dropout layers, 8X4, epsilon_decay = 0.9885, 5 episodes,  ADAM, learning rate: 0.3, update target every 550 learn occurences, discount_factor = 0.99, changed reward *5 for positive, -1 for negative ==> total reward: 0.04
- replay above 20241209 good result
  - **v20241210_0** add back dropout layers, 32X4, epsilon_decay = 0.9885, 5 episodes,  ADAM, learning rate: 0.3, update target every 1000 learn occurences, discount_factor = 0.99 ==> total reward: 0.33 (20 min per episode)
  - add back dropout layers, 32X4, epsilon_decay = 0.9885, 10 episodes,  ADAM, learning rate: 0.3, update target every 1000 learn occurences, discount_factor = 0.99 ==> total reward: 0.082 at episode 5, 0.195 at episode 10 (18.5 mins per episode)
- Think need to update the target network more frequently. might be missing the learnings from the policy network.
  - add back dropout layers, 32X4, epsilon_decay = 0.9885, 30 episodes,  ADAM, learning rate: 0.3, update target every 50 learn occurences, discount_factor = 0.99 ==> total reward: -.11 after 28 episodes (18.5 mins per sepisode)-0.04 after 5 episodes

### 11 Dec

- revert to **v20241210_0**, verify result is with reward modification
  - **v20241211_0** epsilon update after 2000, modified reward *5 positive / -1 else, update target every 2000 learn occurences ==> ***total reward: 0.44***
- revert to **v20241210_0**, verify result
  - without reward modification, ensure update network 1000 learn occ. 3 episodes only, epsilon_decay = 0.93 ==> still doing short only
- experimenting with one episode and get the q-values:
  - 1 episode has very even Q value across all actions ==> Q values all 0 (18 mins)
  - 1 episode try with reward shaping max(1,reward*5) positive else -1 ==> all shorting emphasise Short term rewards too much(18 mins)
  - 1 episode try with reward shaping reward*5 if positive else -1 ==> all shorting still
  - 1 episode try with reward shaping reward if positive else -1 to reward ==> all shorting still
  - 1 episode try with reward shaping reward if positive elif <0 -1 to reward ==> all flat
  - 1 episode try with reward shaping 2*reward if positive elif <0 -1 to reward ==> all flat
  - 1 episode try with reward shaping 5*reward if positive elif <0 -1 to reward ==> all flat
  - 1 episode try with reward shaping +1 reward if positive elif <0 -1 to reward ==> all short
- add back spread as one continuous variable for DQN
  - use ***v20241211_1*** architecture except. learning rate: .3, eps decay: 0.9885, episode: 1, learn every 100 occ, 4x4, ADAM, disc factor: .99 ==> Q-values are different! but seems it is too underfit.. mark this as first 7 input version with some fitting. mark this as ***v20241211_2***
  - v20241211_2 but with learning rate: .9 ==> learning rate too high, oscillating for one gradient from 0 to non-zero through training period
  - v20241211_2 but with learning rate: .5 ==> still underfit as both test and train are negative but small returns
  - v20241211_2 to overcome the underfitting in both previous scenarios plan to increase neurons and decrease learning rate.
    - v20241211_2: 8x8 nn ==> overfit
- seems that it is underfit rather than overfit because, it can get the general trend of mean reversion or choose a general model. First try to train more episodes before changing the params. can afford to reduce the eps decay



### 13 Dec

agent = Agent(num_states=len(env.valid_states),
num_actions=env.num_actions,
batch_size=1000,
alpha=0.1,
gamma=0.9,
epsilon=0.5,
buffer_size = 5000)

:::: episode 10 ::::
========== Absolute ==========
total return 		0.24622
total train return 	0.25300
total test return 	-0.00678
========== Absolute ==========
total return 		0.21601
total train return 	0.22549
total test return 	-0.00948
========== Absolute ==========
total return 		0.13204
total train return 	0.09670
total test return 	0.03534
========== Absolute ==========
total return 		0.13293
total train return 	0.06765
total test return 	0.06528

========== Absolute ==========
total return 		0.12303
total train return 	0.06044
total test return 	0.06258
========== Absolute ==========
total return 		0.17222
total train return 	0.16184
total test return 	0.01037
========== Absolute ==========
total return 		0.22382
total train return 	0.23726
total test return 	-0.01344
---------------------------

agent = Agent(num_states=len(env.valid_states),
num_actions=env.num_actions,
batch_size=1000,
alpha=0.1,
gamma=0.0,
epsilon=0.5,
buffer_size = 5000)
:::: episode 10 ::::
========== Absolute ==========
total return 		-0.15234
total train return 	-0.11700
total test return 	-0.03534
========== Absolute ==========
total return 		0.03424
total train return 	0.07693
total test return 	-0.04270
:::: episode 20 ::::
========== Absolute ==========
total return 		0.00492
total train return 	0.02038
total test return 	-0.01545

---

agent = Agent(num_states=len(env.valid_states),
num_actions=env.num_actions,
batch_size=1000,
alpha=0.01,
gamma=0.0,
epsilon=0.5,
buffer_size = 5000)
:::: episode 50 ::::
========== Absolute ==========
total return 		0.12452
total train return 	0.13897
total test return 	-0.01445

---

agent = Agent(num_states=len(env.valid_states),
num_actions=env.num_actions,
batch_size=30,
alpha=0.1,
gamma=0.9,
epsilon=0.5,
buffer_size = 3000)
:::: episode 10 ::::
========== Absolute ==========
total return 		0.41484
total train return 	0.34088
total test return 	0.07396
========== Absolute ==========
total return 		0.16333
total train return 	0.12247
total test return 	0.04085
========== Absolute ==========
total return 		-0.32168
total train return 	-0.28745
total test return 	-0.03423
poor generalisation of the model

:::: episode 20 ::::
========== Absolute ==========
total return 		-0.09425
total train return 	-0.08103
total test return 	-0.01322

---

agent = Agent(num_states=len(env.valid_states),
num_actions=env.num_actions,
batch_size=60,
alpha=0.01,
gamma=0.0,
epsilon=1,
buffer_size = 10000)
:::: episode 8 ::::
========== Absolute ==========
total return 		0.39454
total train return 	0.32058
total test return 	0.07396

- tried many episode to get generalised results, but little success.
- will try to run long 1000 episodes overnight. will change reward to *5 since range is from -.2 to .2 in a day.

### 14 Dec
- update workflow to run multiple epoch to auto check consistency. did 10 episodes 2 epochs over 37 mins. not too long. 
- running large batch of 50 episodes 3 epochs to see if SD of the returns can reduce.
- reward also observed to be -.2 to +.2 range. with very high 0 values. must find a way to reduce the peakness. for now *5 the reward

- trying reward / reward SD
    - i think this is the right track 
    
- special reward function with more rewards. (30 mins)
                if reward > 2:
                    reward = 2
                elif reward > .5:
                    reward += 1
                elif reward < -2:
                    reward = -2
                elif reward < -.5:
                    reward += -1

`7
3
0.9
0.99
1.0
0.9995
3
3
Evaluation: Average Total Train Return:[-0.020302455715636845, -0.026456559360757727, -0.015957870073517066]
Evaluation: Average Total Train Return: mean -0.0209, SD 0.0043
Evaluation: Average Total Test Return: [0.0, 0.04100139645240182, 0.010911636716948203]
Evaluation: Average Total Test Return: mean 0.0173, SD 0.0173`

- try without scaled rewards

`7
3
0.9
0.99
1.0
0.9995
3
3
Evaluation: Average Total Train Return:[-0.21204523123119973, 0.0, -0.08536008671074533]
Evaluation: Average Total Train Return: mean -0.0991, SD 0.0871
Evaluation: Average Total Test Return: [-0.07280725995434345, 0.0, -0.0437009527948386]
Evaluation: Average Total Test Return: mean -0.0388, SD 0.0299`

- use normalised reward. but just make the episode longer. we can reduce the learning rate now that the reward is on track
10 epidodes with very good SD. 

`7
3
0.1
0.99
1.0
0.9995
3
10
Evaluation: Average Total Train Return:[0.01290827870161031, 0.19459444744142035, 0.05879513532063172]
Evaluation: Average Total Train Return: mean 0.0888, SD 0.0771
Evaluation: Average Total Test Return: [0.007286558280539401, 0.01949411710155653, 0.06346864693540508]
Evaluation: Average Total Test Return: mean 0.0301, SD 0.0241`/
reducing learning rate is important. very long run with much smaller learning rate.

- used normalised reward

`7
3
0.01
0.99
1.0
0.9995
3
10
Evaluation: Average Total Train Return:[-0.0893182425866827, -0.07026478040190354, -0.006342248897760472]
Evaluation: Average Total Train Return: mean -0.0553, SD 0.0355
Evaluation: Average Total Test Return: [0.03880864466354409, -0.05088046311132578, 0.010911636716948203]
Evaluation: Average Total Test Return: mean -0.0004, SD 0.0375
`

- try removing resampling and see performance. but want to do above for longer up to 100 episodes

`7
3
0.01
0.99
1.0
0.9995
1
10
Evaluation: Average Total Train Return:[0.3405281364631186]
Evaluation: Average Total Train Return: mean 0.3405, SD 0.0000
Evaluation: Average Total Test Return: [-0.029588736150981697]
Evaluation: Average Total Test Return: mean -0.0296, SD 0.0000`





### 14 Dec

- update workflow to run multiple epoch to auto check consistency. did 10 episodes 2 epochs over 37 mins. not too long.
- running large batch of 50 episodes 3 epochs to see if SD of the returns can reduce.
- reward also observed to be -.2 to +.2 range. with very high 0 values. must find a way to reduce the peakness. for now *5 the reward
- firm up on gamma 0
- running gamma > 0 experiment
- should still do some reward scaling

### 14 Dec
- experiment with running multiple gamma. 
- ls_gamma = [.5, .1, .2, .9, 0]
    - exp1: Epoch Total Ave Reward: [0.27656047941275835, -0.0678991421108197, -0.08469468350312535, -0.1809340042907028, -0.2585470411630025]
    - exp2: Epoch Total Ave Reward: [0.21300592294405787, 0.31927383581329816, -0.19477593352845785, -0.176282995815822, -0.  042197878268430175]
- ls_gamma = [.9, 0, .5, .1, .2]
    - Epoch Total Ave Reward: [0.31927383581329816, -0.12529959493439907, 0.31927383581329816, -0.2642522721225163, 0.20685181929893712]
- settle on gamma 0.5 . try reward clipping
- increasing reward ||| if reward > 2: reward += 2, elif reward > 1: reward += 1
    - Epoch Total Ave Reward: [-0.01642047141259907, -0.13440731073407086, 0.07929587537637263, 0.2268478521818123, 0.1095582760267271]
    - Epoch Ave Ave Reward: 	0.05297
    - Epoch SD  Ave Reward: 	0.12170 
- clipping reward ||| reward /= dailypnl_sd, reward = np.clip(reward,-1,1) 
    - Epoch Total Ave Reward: [-0.11145766569664421, -0.3913613850599167, 0.14032909285942924, -0.1606315485750662, 0.042197878268430175]
    - Epoch Ave Ave Reward: 	-0.09618
    - Epoch SD  Ave Reward: 	0.18254 
- increase reward and penalty ||| , reward /= dailypnl_sd
            if reward > 2:
                reward += 2
            elif reward > 1:
                reward += 1
            elif reward < -2:
                reward -= 2
            elif reward < -1:
                reward -= 1
    - Epoch Total Ave Reward: [0.21300592294405787, 0.1651926853830304, 0.33396093223633744, -0.1651926853830304, 0.062178140797388294]
    - Epoch Ave Ave Reward: 	0.12183
    - Epoch SD  Ave Reward: 	0.16801 
- combine increase reward and clipping ||| 
            reward /= dailypnl_sd
            if reward > 2:
                reward = 2
            elif reward > .5:
                reward += 1
            elif reward < -2:
                reward = -2
            elif reward < -.5:
                reward += -1
    - exp1 | Epoch Total Ave Reward: [-0.07351450140341671, 0.1760664446499776, -0.20093003717357863, 0.22364736069124266, -0.12529959493439907]
    - Epoch Ave Ave Reward: 	-0.00001
    - Epoch SD  Ave Reward: 	0.16882 
    - exp2 | Epoch Total Ave Reward: [0.23802803428152103, 0.08925582031108967, -0.04759668639518292, 0.03709799710794239, 0.04790310922794429]
    - Epoch Ave Ave Reward: 	0.07294
    - Epoch SD  Ave Reward: 	0.09377 
- combine clipping, also reward shape if further from 0, remove noise
            reward /= dailypnl_sd
            if reward > .1:
                reward = max(2,reward+1)
            elif reward < -.1:
                reward = min(-2, reward-1)
    - bad results
- revert to earlier increase reward experiment and also set alpha to 0.5. later run alpha experiment
    - reward /= dailypnl_sd
            if reward > 2:
                reward +=2
            elif reward > 0.5:
                reward +=1
            elif reward < -2:
                reward += -2
            elif reward < -0.5:
                reward += -1
    - Epoch Total Ave Reward: [0.062178140797388294, 0.18198822677533613, -0.1245877739517566]
    - Epoch Ave Ave Reward: 	0.03986
    - Epoch SD  Ave Reward: 	0.12615 
---
- rerun all with newest reward function
    - reward /= dailypnl_sd
            if reward > 2:
                reward +=2
            elif reward > 0.5:
                reward +=1
            elif reward < -2:
                reward += -2
            elif reward < -0.5:
                reward += -1
    - Epoch Total Ave Reward: [0.05321204568778015, -0.06916991576129747, -0.06925978742821415]
    - Epoch Ave Ave Reward: 	-0.02841
    - Epoch SD  Ave Reward: 	0.05771
    - really bad returns on both gamma =0 and >0. alpha was lower in =0.
    - might be under fitting the reward. maybe clip the punishment and leave the rewards
- rerun with removing the negative returns
    - not good result
- rerun with no reward scaling, only normalisation
    - small bactch run
    - gamma =0 || Epoch Total Ave Reward: [-0.14945136647535748, -0.04198132710258564, -0.09593289795481606]
        - Epoch Ave Ave Reward: 	-0.09579
        - Epoch SD  Ave Reward: 	0.04387 
    - gamma <0 || Epoch Total Ave Reward: [0.1921647228758977, 0.1920243147234945, 0.02189542255279333]
        - Epoch Ave Ave Reward: 	0.13536
        - Epoch SD  Ave Reward: 	0.08023
- rerun with no reward scaling, only normalisation
    - large bactch run
        - gamma=0 || Epoch Total Ave Reward: [-0.14352958434999935, 0.13691287936579682, -0.023256067870187767]
        - Epoch Ave Ave Reward: 	-0.00996
        - Epoch SD  Ave Reward: 	0.11488 
        - gamma=0.5 || Epoch Total Ave Reward: [0.1809340042907028, 0.17501222216534448, -0.10711308005452447]
        Epoch Ave Ave Reward: 	0.08294
        Epoch SD  Ave Reward: 	0.13441 
    - definitely not converged yet
- smaller batch run with smaller alpha
    - gamma=0 ||
        - Epoch Total Ave Reward: [0.2704063757676372, 0.043035549587218724, -0.08364046101849247, 0.2674382057968297, -0.08521765750173113, 0.1054302415504514]
        - Epoch Ave Ave Reward: 	0.08624
        - Epoch SD  Ave Reward: 	0.14563 
    - gamma=0.5 || 
        - Epoch Total Ave Reward: [0.07997502788129901, 0.1448902296673933, 0.23595556798148493]
        - Epoch Ave Ave Reward: 	0.15361
        - Epoch SD  Ave Reward: 	0.06398 
- larger batch run with smaller alpha
    - gamma=0
        - Epoch Total Ave Reward: [0.05913382781313888, -0.1809340042907028, -0.12322712863436254, -0.01642047141259907, 0.03731454827378737]
        - Epoch Ave Ave Reward: 	-0.04483
        - Epoch SD  Ave Reward: 	0.09277 
    - gamma=0.5
        - Epoch Total Ave Reward: [-0.09593289795481606, 0.339576291528935, -0.023256067870187767, 0.20388364932812958, -0.22250326653969316]
    - Epoch Ave Ave Reward: 	0.04035
    - Epoch SD  Ave Reward: 	0.20396 