all: lib.a  
	gcc -o nms main.c lib.a -lm -lpcap -lnet -lpthread

lib.a:
	gcc -c egress_convert.c -o egress.o
	gcc -c generate_rules.c -o generate.o
	gcc -c seperate_rules.c -o seperate.o
	gcc -c ip_mac_change.c	-o ip_mac_change.o
	gcc -c ipv6.c -o ipv6.o
	gcc -c MAC.c -o MAC.o
	gcc -c RLOC_stack_up.c -o stack_up.o
	gcc -c get_new_a.c -lm -o get_new_a.o
	gcc -c two_to_ten.c -lm -o two_to_ten.o
	gcc -c ten_to_two.c -lm -o ten_to_two.o
	gcc -c get_mask.c -lm -o get_mask.o
	gcc -c build_addr.c -lm -o build_addr.o
	gcc -c check.c -lm -o check.o
	gcc -c addr_to_rule.c -lm -o addr_to_rule.o
	gcc -c get_vector.c -lm -o get_vector.o
	#gcc -c write_action_data.c -lpcap -lnet -o write_action_data.o
	gcc -c add_actions.c -o add_actions.o
	#gcc -c match.c -o match.o
	gcc -c configure_rule_action.c -o configure_rule_action.o
	gcc -c get_bv.c -o get_bv.o
	gcc -c nmachandle.c -lpcap -lnet -o nmachandle.o
	gcc -c write_table.c -lpcap -lnet -o write_table.o
	gcc -c aux_table.c -lpcap -lnet -lpthread -o aux_table.o
	gcc -c ofp_demo.c -lpcap -lnet -lpthread -o ofp_demo.o
	ar -rcs lib.a egress.o generate.o seperate.o ip_mac_change.o stack_up.o addr_to_rule.o ipv6.o get_new_a.o ten_to_two.o two_to_ten.o get_mask.o build_addr.o check.o get_vector.o add_actions.o  MAC.o configure_rule_action.o get_bv.o nmachandle.o write_table.o aux_table.o ofp_demo.o


clean:
	rm -rf *.o lib.a main
