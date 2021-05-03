/*    /daemon/services/tell.c
 *    from the Dead Souls LPC Library
 *    handles the IMP tell service
 *    created by Descartes of Borg 950507
 */

#define SERVICE_TELL

#include <daemons.h>

void eventReceiveTell(mixed *packet) {
    object ob, machine;
    string who;
    string adverb = "";

    if( file_name(previous_object()) != INTERMUD_D ) return;
    who = convert_name(packet[5]);
    if( !(ob = find_player(who)) || (int)ob->query_invisible() ) {
	INTERMUD_D->eventWrite(({ "error", 5, mud_name(), 0, packet[2],
	    packet[3], "unk-user", 
	    capitalize(packet[5]) + " is nowhere to "
	    "be found on " + mud_name() + ".",
	    packet }));
	if(!(ob = find_player(who))) return;
	adverb = " %^BOLD%^MAGENTA%^unknowingly%^BOLD%^RED%^";
    }

    machine=present("answering machine",ob);
    if(machine && base_name(machine) == "/secure/obj/machine"){
	int parse_it;
	string machine_message;
	parse_it=machine->query_answer();
	if(parse_it && !(int)ob->query_invisible()){
	    machine->get_message(packet[6] + "@" + packet[2]+
	      " tells you: "+packet[7]+"\n");
	    machine_message=machine->send_message();
	    INTERMUD_D->eventWrite(({ "error", 5, mud_name(), 0, packet[2],
		packet[3], "unk-user",
		machine_message,
		packet }));
	    return;
	}
    }

    message("tell","%^BOLD%^RED%^" + packet[6] + "@" + packet[2] +
      adverb + " tells you:%^RESET%^ " + packet[7], ob);
    if(!sizeof(adverb)) ob->SetProperty("reply", packet[6] + "@" + packet[2]);
}

void eventSendTell(string who, string where, string msg) {
    string pl, plc;

    pl = (string)this_player(1)->query_name();
    plc = (string)this_player(1)->query_cap_name();
    where = (string)INTERMUD_D->GetMudName(where);
    INTERMUD_D->eventWrite(({ "tell", 5, mud_name(), pl, where, 
	convert_name(who), plc, msg }));
    message("tell", "%^BOLD%^RED%^You tell " + capitalize(who) + "@" +
      where + ":%^RESET%^ " + msg+"\n",this_player(1));
}




