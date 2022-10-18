#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>


using namespace std;
class wire{
public:
  string faulty_wire;
  string fault_value;
  string value;
  string name;
  public:
    wire(string f_w , string f_v , string n) : faulty_wire(f_w) , fault_value(f_v) , value("X") , name(n) {};
    wire(string n) : value("X") , name(n) {};
    wire() : value("X") {};
    void put(string a){ value = a; }
    void get(string& a){ a = value; }
};

class Gate{
protected:
  wire *i1 , *i2 , *o1;
public:
  string gate_name;
  Gate(wire& a , wire& w): i1(&a) , o1(&w) {}
  Gate(wire& a , wire& b , wire& w): i1(&a) , i2(&b) , o1(&w) {}
  Gate() {};
  ~Gate() {};
  void eval(string gate_type);
  void print_list_ports(string gate_type){
    if(gate_type != "not"){
      cout << "first input is : " << i1->name << "  its value is: " << i1->value << endl;
      cout << "second input is : " << i2->name << "  its value is: " << i2->value << endl;
      cout << "output is : " << o1->name << "  its value is: " << o1->value << endl;
    }
    else if(gate_type == "not"){
      cout << "input is : " << i1->name << "  its value is: " << i1->value << endl;
      cout << "output is : " << o1->name << "  its value is: " << o1->value << endl;
    }
  }
  vector<wire> save_list_ports(string gate_type){
    vector<wire> arr;
    if(gate_type != "not"){
      arr.push_back(*i1);
      arr.push_back(*i2);
      arr.push_back(*o1);
    }
    else if(gate_type == "not"){
      arr.push_back(*i1);
      arr.push_back(*o1);
    }
    return arr;
  }

};
void Gate::eval(string gate_type){  

    if(gate_type == "not"){
      if(i1->faulty_wire == "1")
        i1->value = i1->fault_value;
      if(o1->faulty_wire == "1")
          o1->value = o1->fault_value; 
      else if(i1->value == "1")
        o1->value = "0";
      else if(i1->value == "0")
        o1->value = "1";
      else
        o1->value = "X";
    }
    if(gate_type == "nand"){
      if(i1->faulty_wire == "1")
        i1->value = i1->fault_value;
      if(i2->faulty_wire == "1")
        i2->value = i2->fault_value;
      if(o1->faulty_wire == "1")
          o1->value = o1->fault_value;
      else if((i1->value == "0") || (i2->value == "0"))
          o1->value = "1";
      else if((i1->value == "1") && (i2->value == "1"))
          o1->value = "0";
      else
          o1->value = "X";
    }
    if(gate_type == "nor"){
      if(i1->faulty_wire == "1")
        i1->value = i1->fault_value;
      if(i2->faulty_wire == "1")
        i2->value = i2->fault_value;
      if(o1->faulty_wire == "1")
          o1->value = o1->fault_value;
      else if((i1->value == "1") || (i2->value == "1"))
          o1->value = "0";
      else if((i1->value == "0") && (i2->value == "0"))
          o1->value = "1";
      else
          o1->value = "X";
    }

}

class NAND : public Gate{
public:
  NAND(wire& a , wire& b , wire& w): Gate(a,b,w){}
  ~NAND();
};
class NOR : public Gate{
public:
  NOR(wire& a , wire& b , wire& w): Gate(a,b,w){}
  ~NOR();
};
class NOT : public Gate{
public:
  NOT(wire& a , wire& w): Gate(a,w){}
  ~NOT();
};


vector<string> get_lines_of_file(vector<string> lines , string file_name);
vector<string> string_to_word(string str);
void inputs_outputs(vector<vector<string> >& in_out , vector<string> gate_lines);


vector<string> figure_inputs(vector<string> input_lines);
vector<string> figure_outputs(vector<string> output_lines);
vector<string> figure_gate_names(vector<string> gate_lines);
vector<wire> seq_inputs(string first_line , vector<string> input_list);
vector<wire> figure_outputs_wires(vector<string> output_lines);
vector<wire> figure_wires_wires(vector<string> wire_lines);

vector<wire> initial_inputs(vector<wire> sequenced_inputs_wire , string tb_line);
vector<Gate*> figure_gates(vector<vector<string> > inouts , vector<wire*> all_wires_address , vector<string> gate_lines);

void simulate_circuit(vector<wire> outputs_wire , vector<Gate*> gate_list_address , vector<string> gates);

int where_in_vector_pointer(string wire_name , vector<wire*> wire_vect_adr);
int where_in_vector(string wire_name , vector<wire> wire_vect);
bool checked_befor(vector<wire> fault_vector , wire checking_wire);
bool output_ready(vector<wire> outputs_wire);
vector<string> save_output_results(vector<wire> outputs_wire);
bool similar_outputs(vector<string> wanted_output_result , vector<string> actual_output_result);

vector<string> figure_gates(vector<string> gate_lines);

void find_fan_out_fault(vector<wire>& fault_vector , vector<vector<string> > inouts , vector<string> gates , vector<wire> all_wires);
void find_primery_output_file(vector<wire>& fault_vector , vector<wire> outputs_wire , vector<wire> all_wires);
void find_nand_nor_faults(vector<wire>& fault_vector , vector<string> gates , vector<wire> all_wires , vector<Gate*> gate_list_address);
void generate_txt_faults(vector<wire> fault_vector , vector<Gate*> gate_list_address , vector<string> gates , vector<string> outs);
string find_gate_name_for_port(vector<Gate*> gate_list_address , string wanted_name , vector<string> gates , string val_fault , vector<string> outs);

vector<wire*> reset_circuit(vector<wire*> wire_vect_adr , vector<string> inputs);
void print_is_error(vector<string> wanted_output_result , vector<string> actual_output_result);


int main(){

  vector<string> file_lines;
  vector<string> simu_lines;
    file_lines = get_lines_of_file(file_lines , "ooTest2.txt");
    simu_lines = get_lines_of_file(simu_lines , "testbench.txt");
    vector<string> input_lines;
    vector<string> output_lines;
    vector<string> wire_lines;
    vector<string> gate_lines;
    
    vector<string> words;
    vector<string> inputs;
    vector<wire> sequenced_inputs_wire;
    vector<wire> outputs_wire;
    vector<wire> wires_wire;
    vector<wire> all_wires;
    vector<wire*> all_wires_address;

    vector<string> gates;
    vector<string> outs;

    for(int i = 0 ; i < file_lines.size() ; i++){
      words = string_to_word(file_lines[i]);
        if(words[0] == "input")
          input_lines.push_back(file_lines[i]);
        if(words[0] == "output")
          output_lines.push_back(file_lines[i]);
      if(words[0] == "wire")
          wire_lines.push_back(file_lines[i]);
        if(words[0] == "nand" || words[0] == "nor" || words[0] == "not" )
          gate_lines.push_back(file_lines[i]);
        words.clear();
    }

    vector<vector<string> > inouts(gate_lines.size() , vector<string> (4));
    inputs_outputs(inouts , gate_lines);
    outs = figure_outputs(output_lines);
    inputs = figure_inputs(input_lines);
    gates = figure_gate_names(gate_lines);
    sequenced_inputs_wire = seq_inputs(file_lines[0] , inputs);
    outputs_wire = figure_outputs_wires(output_lines);
    wires_wire = figure_wires_wires(wire_lines);
    ofstream new_file("SimulationResults.txt");

    for(int loop = 0 ; loop < simu_lines.size() ; loop ++){
        sequenced_inputs_wire = initial_inputs(sequenced_inputs_wire , simu_lines[loop]);


        //creat a vector of wire class
        for(int i = 0 ; i < sequenced_inputs_wire.size() ; i++){
          all_wires.push_back(sequenced_inputs_wire[i]);
        }
        for(int i = 0 ; i < wires_wire.size() ; i++){
          all_wires.push_back(wires_wire[i]);
        }
        for(int i = 0 ; i < outputs_wire.size() ; i++){
          all_wires.push_back(outputs_wire[i]);
        }


        //creat a vector of wire calss pointers
        for(int i = 0 ; i < sequenced_inputs_wire.size() ; i++){
          all_wires_address.push_back(&sequenced_inputs_wire[i]);
        }
        for(int i = 0 ; i < wires_wire.size() ; i++){
          all_wires_address.push_back(&wires_wire[i]);
        }
        for(int i = 0 ; i < outputs_wire.size() ; i++){
          all_wires_address.push_back(&outputs_wire[i]);
        }
 
        //creat a vector of gate clss pointers
        vector<Gate*> gate_list_address;
        gate_list_address = figure_gates(inouts , all_wires_address , gate_lines);
        //generate vector of fault wires
        vector<wire> fault_vector;
        find_nand_nor_faults(fault_vector , gates , all_wires , gate_list_address);
        find_fan_out_fault(fault_vector , inouts , gates , all_wires);
        find_primery_output_file(fault_vector , outputs_wire , all_wires);
        generate_txt_faults(fault_vector , gate_list_address , gates , outs);
        //simulate circuit with no fault
        simulate_circuit(outputs_wire , gate_list_address , gates);
        vector<string> wanted_output_result;
        wanted_output_result = save_output_results(outputs_wire);
        //test circuit
        new_file << "------------------------------------------------------------------------------" << endl;
        for(int k = 0 ; k < sequenced_inputs_wire.size() ; k++){
          new_file << "input name :   " << sequenced_inputs_wire[k].name << "     input value :  " << sequenced_inputs_wire[k].value << endl;
        }
        for(int k = 0 ; k < wanted_output_result.size() ; k++){
          new_file << "output name :   " << outputs_wire[k].name << "     wanted value :  " << wanted_output_result[k] << endl;
        }
        new_file << "------------------------------------------------------------------------------" << endl;

        for(int i = 0 ; i < fault_vector.size() ; i++){
          int where = where_in_vector_pointer(fault_vector[i].name , all_wires_address);
          string true_value = all_wires_address[where]->value;
          all_wires_address[where]->faulty_wire = fault_vector[i].faulty_wire;
          all_wires_address[where]->fault_value = fault_vector[i].fault_value;
          all_wires_address = reset_circuit(all_wires_address , inputs);
          simulate_circuit(outputs_wire , gate_list_address , gates);
          vector<string> actual_output_result = save_output_results(outputs_wire);

          
          new_file << "faulty wire:  " << fault_vector[i].name << "    fault value:   " << fault_vector[i].fault_value << endl;
          for(int k = 0 ; k < actual_output_result.size() ; k++){
            new_file << "output name :   " << outputs_wire[k].name << "     actual value :  " << actual_output_result[k] << endl;
          }
          if(similar_outputs(wanted_output_result , actual_output_result)){
            new_file << "no error in final ans" << endl;
          }
          else
            new_file << "error in final ans" << endl;
          new_file << "**************************" << endl;


          where = where_in_vector_pointer(fault_vector[i].name , all_wires_address);
          all_wires_address[where]->value = true_value;
          all_wires_address[where]->faulty_wire = "0";
          all_wires_address[where]->fault_value = "0";
          all_wires_address = reset_circuit(all_wires_address , inputs);
          simulate_circuit(outputs_wire , gate_list_address , gates);

        }
    //reset circuit totaly for new inputs
    for(int x = 0 ; x < all_wires_address.size() ; x++){
      all_wires_address[x]->value = "X";
      all_wires_address[x]->faulty_wire = "0";
      all_wires_address[x]->fault_value = "0";
    }
    all_wires.clear();
    all_wires_address.clear();
    gate_list_address.clear();
    fault_vector.clear();

    }

    
    new_file.close();


}




vector<wire*> reset_circuit(vector<wire*> wire_vect_adr , vector<string> inputs){
  for(int i = 0 ; i < wire_vect_adr.size() ; i++){
    if(find(inputs.begin(), inputs.end(), wire_vect_adr[i]->name) != inputs.end()) 
        continue;
    else 
      wire_vect_adr[i]->value = "X";    
    
  }
  return wire_vect_adr;
}

void simulate_circuit(vector<wire> outputs_wire , vector<Gate*> gate_list_address , vector<string> gates){
    int cnt = 0;
    while(output_ready(outputs_wire) != true){
      for(int i = 0 ; i < gate_list_address.size() ; i++){
        gate_list_address[i]->eval(gates[i]);
      }
      cnt++;
      if(cnt == 20)
        break;

    }

}

bool similar_outputs(vector<string> wanted_output_result , vector<string> actual_output_result){
  for(int i = 0 ; i < wanted_output_result.size() ; i++){
    if(wanted_output_result[i] != actual_output_result[i])
      return false;
  }
  return true;
}



vector<string> save_output_results(vector<wire> outputs_wire){
  vector<string> out_vals;
  for(int i = 0 ; i < outputs_wire.size() ; i++){
    out_vals.push_back(outputs_wire[i].value);
  }
  return out_vals;
}

void generate_txt_faults(vector<wire> fault_vector , vector<Gate*> gate_list_address , vector<string> gates , vector<string> outs){
  ofstream my_file("PossibleFaults.txt");
  string port_connected_to;
  for(int i = 0 ; i < fault_vector.size() ; i++){
    port_connected_to = find_gate_name_for_port(gate_list_address , fault_vector[i].name , gates , fault_vector[i].fault_value , outs);
    my_file << "wire name : " << fault_vector[i].name << endl;
    my_file << "fault value :    " << fault_vector[i].fault_value << endl;
    my_file << "connected to  " << port_connected_to << endl;
    my_file << "****************************" << endl;
  }
  my_file.close();
}

string find_gate_name_for_port(vector<Gate*> gate_list_address , string wanted_name , vector<string> gates , string val_fault , vector<string> outs){
  for(int i = 0 ; i < gate_list_address.size() ; i++){
    if(gates[i] == "not")
      continue;
    else{
      vector<wire> ports = gate_list_address[i]->save_list_ports(gates[i]);
      ports.pop_back();
      for(int j = 0 ; j < ports.size() ; j++){
        if(ports[j].name == wanted_name){
          if( (val_fault == "1" && gates[i] == "nand") || (val_fault == "0" && gates[i] == "nor") )
            return gates[i];
          else
            return "fan_out";
        }
      }
    }
  }
  return "primary output";
}

void find_nand_nor_faults(vector<wire>& fault_vector , vector<string> gates , vector<wire> all_wires , vector<Gate*> gate_list_address){
  for(int i = 0 ; i < gates.size() ; i++){


      if(gates[i] == "nand"){
        vector<wire> possible_fault_wire = gate_list_address[i]->save_list_ports(gates[i]);
        int index1 = where_in_vector(possible_fault_wire[0].name , all_wires);
        int index2 = where_in_vector(possible_fault_wire[1].name , all_wires);
        if(index1 == index2){
          all_wires[index1].fault_value = "1";
          all_wires[index1].faulty_wire = "1";
          if(checked_befor(fault_vector , all_wires[index1]) != true){
            fault_vector.push_back(all_wires[index1]);
          }
        }
        else if(index1 != index2){
          all_wires[index1].fault_value = "1";
          all_wires[index1].faulty_wire = "1";
          if(checked_befor(fault_vector , all_wires[index1]) != true){
            fault_vector.push_back(all_wires[index1]);
          }
          all_wires[index2].fault_value = "1";
          all_wires[index2].faulty_wire = "1";
          if(checked_befor(fault_vector , all_wires[index2]) != true){
            fault_vector.push_back(all_wires[index2]);
          }
        }
      }



      if(gates[i] == "nor"){
        vector<wire> possible_fault_wire = gate_list_address[i]->save_list_ports(gates[i]);
        int index1 = where_in_vector(possible_fault_wire[0].name , all_wires);
        int index2 = where_in_vector(possible_fault_wire[1].name , all_wires);
        if(index1 == index2){
          all_wires[index1].fault_value = "0";
          all_wires[index1].faulty_wire = "1";
          if(checked_befor(fault_vector , all_wires[index1]) != true){
            fault_vector.push_back(all_wires[index1]);
          }
        }
        else if(index1 != index2){
          all_wires[index1].fault_value = "0";
          all_wires[index1].faulty_wire = "1";
          if(checked_befor(fault_vector , all_wires[index1]) != true){
            fault_vector.push_back(all_wires[index1]);
          }
          all_wires[index2].fault_value = "0";
          all_wires[index2].faulty_wire = "1";
          if(checked_befor(fault_vector , all_wires[index2]) != true){
            fault_vector.push_back(all_wires[index2]);
          }
        }
      }



      if(gates[i] == "not")
        continue;
  }
}

void find_primery_output_file(vector<wire>& fault_vector , vector<wire> outputs_wire , vector<wire> all_wires){
  for(int i = 0 ; i < outputs_wire.size() ; i++){
    int index = where_in_vector(outputs_wire[i].name , all_wires);
    all_wires[index].fault_value = "1";
    all_wires[index].faulty_wire = "1";
    if(checked_befor(fault_vector , all_wires[index]) != true){
      fault_vector.push_back(all_wires[index]);
    }
    all_wires[index].fault_value = "0";
    all_wires[index].faulty_wire = "1";
    if(checked_befor(fault_vector , all_wires[index]) != true){
      fault_vector.push_back(all_wires[index]);
    }

  }
}

void find_fan_out_fault(vector<wire>& fault_vector , vector<vector<string> > inouts , vector<string> gates , vector<wire> all_wires){
  vector<string>in_names;
  for(int i = 0 ; i < gates.size() ; i++){
    if(gates[i] == "not"){
      inouts[i].pop_back();
      in_names.push_back(inouts[i].back());
    }
    else if(gates[i] != "not"){
      in_names.push_back(inouts[i].back());
      inouts[i].pop_back();
      in_names.push_back(inouts[i].back());
    }
  }
 
  for(int i = 0 ; i < in_names.size() ; i++){
    if( count(in_names.begin() , in_names.end() , in_names[i]) > 1 ){
      int index = where_in_vector(in_names[i] , all_wires);
      all_wires[index].fault_value = "1";
      all_wires[index].faulty_wire = "1";
      if(checked_befor(fault_vector , all_wires[index]) != true){
        fault_vector.push_back(all_wires[index]);
      }
      all_wires[index].fault_value = "0";
      all_wires[index].faulty_wire = "1";
      if(checked_befor(fault_vector , all_wires[index]) != true){
        fault_vector.push_back(all_wires[index]);
      }
    }
  }
}

bool checked_befor(vector<wire> fault_vector , wire checking_wire){
  for(int i = 0 ; i < fault_vector.size() ; i++){
    if(fault_vector[i].name == checking_wire.name){
      if(fault_vector[i].fault_value == checking_wire.fault_value)
        return true;
    }
  }
  return false;
}

vector<string> figure_gate_names(vector<string> gate_lines){
  vector<string> gate;
  vector<string> words;
  for(int i = 0 ; i < gate_lines.size() ; i++){
      words = string_to_word(gate_lines[i]);
      gate.push_back(words[0]);
      words.clear();
    }
    return gate;
}


vector<Gate*> figure_gates(vector<vector<string> > inouts , vector<wire*> all_wires_address , vector<string> gate_lines){
  int index1 , index2 , index3;
  vector<Gate*> gate_list_address;
  for(int i = 0 ; i < gate_lines.size() ; i++){
      if(inouts[i][0] == "not")
        index1 = where_in_vector_pointer(inouts[i][2] , all_wires_address);
      else
        index1 = where_in_vector_pointer(inouts[i][3] , all_wires_address);

      index2 = where_in_vector_pointer(inouts[i][2] , all_wires_address);
      index3 = where_in_vector_pointer(inouts[i][1] , all_wires_address);

      if(inouts[i][0] == "nand"){
        Gate *nand_gate = new NAND(*all_wires_address[index1] , *all_wires_address[index2] , *all_wires_address[index3]);
        nand_gate->gate_name = "nand";
        gate_list_address.push_back(nand_gate);
      }
      if(inouts[i][0] == "nor"){
        Gate *nor_gate = new NOR(*all_wires_address[index1] , *all_wires_address[index2] , *all_wires_address[index3]);
        nor_gate->gate_name = "nor";
        gate_list_address.push_back(nor_gate);
      }
      if(inouts[i][0] == "not"){
        Gate *not_gate = new NOT(*all_wires_address[index1] , *all_wires_address[index3]);
        not_gate->gate_name = "not";
        gate_list_address.push_back(not_gate);
      }

    }
  return gate_list_address;
}



bool output_ready(vector<wire> outputs_wire){
  for(int i = 0 ; i < outputs_wire.size() ; i++){
    if((outputs_wire[i].value == "0") || (outputs_wire[i].value == "1"))
      continue;
    else
      return false;
  }
  return true;
}


int where_in_vector_pointer(string wire_name , vector<wire*> wire_vect_adr){
  for(int i = 0 ; i < wire_vect_adr.size() ; i++){
    if(wire_vect_adr[i]->name == wire_name)
      return i;
  }
  return -1;
}
int where_in_vector(string wire_name , vector<wire> wire_vect){
  for(int i = 0 ; i < wire_vect.size() ; i++){
    if(wire_vect[i].name == wire_name)
      return i;
  }
  return -1;
}


void inputs_outputs(vector<vector<string> >& in_out , vector<string> gate_lines){
  vector<string> words;
    for(int i = 0 ; i < gate_lines.size() ; i++){
        words = string_to_word(gate_lines[i]);
        if(words[1] != "#"){
          if(in_out[i][0] != "not"){
            in_out[i][0] = words[0];
            in_out[i][1] = words[2];
            in_out[i][2] = words[3];
            in_out[i][3] = words[4];
          }
          else if(in_out[i][0] == "not"){
            in_out[i][0] = words[0];
            in_out[i][1] = words[2];
            in_out[i][2] = words[3];
            in_out[i][3] = words[3];
          }
        }
        else{
          if(in_out[i][0] != "not"){
            in_out[i][0] = words[0];
            in_out[i][1] = words[5];
            in_out[i][2] = words[6];
            in_out[i][3] = words[7];
          }
          else if(in_out[i][0] == "not"){
            in_out[i][0] = words[0];
            in_out[i][1] = words[5];
            in_out[i][2] = words[6];
            in_out[i][3] = words[6];
          } 
        }
        words.clear();
    }
}


vector<wire> initial_inputs(vector<wire> sequenced_inputs_wire , string tb_line){
  vector<string> line_to_word;
  string input_vals;
    line_to_word = string_to_word(tb_line);
    input_vals = line_to_word[1];
    for(int i = 0 ; i < sequenced_inputs_wire.size() ; i++){
      sequenced_inputs_wire[i].value = input_vals[i];
    }
    return sequenced_inputs_wire;
}

vector<wire> seq_inputs(string first_line , vector<string> input_list){
  vector<string> line_to_word;
    vector<string> sequenced;
    vector<wire> in;
    line_to_word = string_to_word(first_line);
    for(int i = 2 ; i < line_to_word.size() ; i++){
      if (find(input_list.begin(), input_list.end(), line_to_word[i]) != input_list.end())
        sequenced.push_back(line_to_word[i]);
    } 
    for(int i = 0 ; i < sequenced.size() ; i++){
      in.push_back(wire( "0" , "0" , sequenced[i]));
    }
    return in;
}

vector<wire> figure_wires_wires(vector<string> wire_lines){
  vector<wire> wire_wires;
    vector<string> words;
    for(int i = 0 ; i < wire_lines.size() ; i++){
        words = string_to_word(wire_lines[i]);
        wire_wires.push_back(wire( "0" , "0" , words[1]));
        words.clear();
    }
    return wire_wires;
}


vector<wire> figure_outputs_wires(vector<string> output_lines){
  vector<wire> out_wires;
    vector<string> words;
    for(int i = 0 ; i < output_lines.size() ; i++){
        words = string_to_word(output_lines[i]);
        out_wires.push_back(wire( "0" , "0" ,words[1]));
        words.clear();
    }
    return out_wires;
}



vector<string> figure_inputs(vector<string> input_lines){
  vector<string> inp;
    vector<string> words;
    for(int i = 0 ; i < input_lines.size() ; i++){
      words = string_to_word(input_lines[i]);
        inp.push_back(words[1]);
        words.clear();
    }
    return inp;
}

vector<string> figure_outputs(vector<string> output_lines){
  vector<string> out;
  vector<string> words;
  for(int i = 0 ; i < output_lines.size() ; i++){
      words = string_to_word(output_lines[i]);
      out.push_back(words[1]);
      words.clear();
    }
    return out;

}


vector<string> string_to_word(string str){
  vector<string> words; 
    string word = ""; 
    for (auto x : str)  { 
      if (x == ' ' || x == '(' || x == ')' || x == ',' || x == ';' || x == '\n'){ 
          if(word != "")  
            words.push_back(word);
            word = ""; 
      } 
      else{ 
        word = word + x; 
      } 
    }    
  words.push_back(word);
    return words; 
} 

vector<string> get_lines_of_file(vector<string> lines , string file_name){

    ifstream in(file_name);

    if(!in) {
        cout << "Cannot open input file.\n";
        return lines;
    }

    char str[255];

    while(in) {
        in.getline(str, 255); 
        if(in) 
          lines.push_back(str);
    }

    in.close();
    return lines;
}