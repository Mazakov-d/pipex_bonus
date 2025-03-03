NAME = pipex
NAME_BONUS = pipex_bonus

F_PIPEX_DIR = f_pipex
F_PIPEX_BONUS_DIR = f_pipex_bonus

F_PIPEX_BUILD = $(F_PIPEX_DIR)/.build_complete
F_PIPEX_BONUS_BUILD = $(F_PIPEX_BONUS_DIR)/.build_complete

all: $(NAME)

$(NAME): $(F_PIPEX_BUILD)
	@touch $(F_PIPEX_BUILD)

$(F_PIPEX_BUILD): FORCE
	$(MAKE) -C $(F_PIPEX_DIR)
	@touch $@

bonus: $(NAME_BONUS)

$(NAME_BONUS): $(F_PIPEX_BONUS_BUILD)
	@touch $(F_PIPEX_BONUS_BUILD)

$(F_PIPEX_BONUS_BUILD): FORCE
	$(MAKE) -C $(F_PIPEX_BONUS_DIR) 
	@touch $@

clean:
	$(MAKE) -C $(F_PIPEX_DIR) clean
	$(MAKE) -C $(F_PIPEX_BONUS_DIR) clean
	@rm -f $(F_PIPEX_BUILD) $(F_PIPEX_BONUS_BUILD)

fclean:
	$(MAKE) -C $(F_PIPEX_DIR) fclean
	$(MAKE) -C $(F_PIPEX_BONUS_DIR) fclean
	@rm -f $(F_PIPEX_BUILD) $(F_PIPEX_BONUS_BUILD)

re: fclean all

re_bonus: fclean bonus

FORCE:

.PHONY: all bonus clean fclean re re_bonus FORCE