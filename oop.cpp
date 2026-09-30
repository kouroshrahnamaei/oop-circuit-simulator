#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <cmath>
#include "algorithm"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL2_gfx.h>
#include "vector"
#include <string>
#include <fstream>
#include <bits/stdc++.h>
#include <unordered_map>
#include <iomanip>

std::string gProjectPath;

using namespace std;
class MonfaredMNA : public runtime_error {
public:
    MonfaredMNA() : runtime_error("Error: MNA matrix is singular (determinant is zero or near zero). Cramer's rule cannot solve it.\n") {}
};
class VoltageSourceBeinDoG : public runtime_error {
public:
    VoltageSourceBeinDoG(const string& source_name) : runtime_error("Error: Voltage source " + source_name + " connected between two ground nodes. This creates an inconsistent or redundant circuit.\n") {}
};
class AndisGozariFailed : public runtime_error {
public:
    AndisGozariFailed() : runtime_error("Error: Circuit initialization failed within SolveCircuit.\n") {}
};
class NoGround : public runtime_error {
public:
    NoGround() : runtime_error("Error: No ground node found or circuit is empty. Cannot initialize circuit structure.\n") {}
};
class InvalidTimeStep : public runtime_error {
public:
    InvalidTimeStep() : runtime_error("Error: Simulation time step (dt) is invalid or not set.\n") {}
};
class ManfiTimeKol : public runtime_error {
public:
    ManfiTimeKol() : runtime_error("Error: Total simulation time (t) is negative.\n") {}
};
class SolveCircuitFailed : public runtime_error {
public:
    SolveCircuitFailed(double time_t) : runtime_error("Error: Circuit solution failed at time t = " + to_string(time_t) + " s\n") {}
};
class InvalidDCSweep : public runtime_error {
public:
    InvalidDCSweep() : runtime_error("Error: DC Sweep source is not set or invalid.\n") {}
};
class TimeStepDCZero : public runtime_error {
public:
    TimeStepDCZero() : runtime_error("Error: DC Sweep increment cannot be zero.\n") {}
};
class NoOutDCSweep : public runtime_error {
public:
    NoOutDCSweep() : runtime_error("Error: No outputs specified for DC Sweep.\n") {}
};
class NotFoundSDCSweep : public runtime_error {
public:
    NotFoundSDCSweep(const string& source_name) : runtime_error("Error: Source " + source_name + " not found for .DC sweep.\n") {}
};
class InvalidDCSweepType : public runtime_error {
public:
    InvalidDCSweepType(const string& source_name) : runtime_error("Error: Source " + source_name + " is not a valid voltage or current source for .DC sweep.\n") {}
};
class MajaziDCSweepComp : public runtime_error {
public:
    MajaziDCSweepComp(const string& source_name) : runtime_error("Error: Source " + source_name + " is a virtual component and cannot be swept.\n") {}
};
class MatchStepDCSweep : public runtime_error {
public:
    MatchStepDCSweep() : runtime_error("Error: .DC sweep step does not match start/end values.\n") {}
};
class NotFoundComponent : public runtime_error {
public:
    NotFoundComponent(const string& component_name) : runtime_error("Error: Element " + component_name + " not found in library\n") {}
};
class TecResistor : public runtime_error {
public:
    TecResistor(const string& name) : runtime_error("Error: Resistor " + name + " already exists in the circuit\n") {}
};
class TecVS : public runtime_error {
public:
    TecVS(const string& name) : runtime_error("Error: VoltageSource " + name + " already exists in the circuit\n") {}
};
class TecCS : public runtime_error {
public:
    TecCS(const string& name) : runtime_error("Error: CurrentSource " + name + " already exists in the circuit\n") {}
};
class TecCapacitor : public runtime_error {
public:
    TecCapacitor(const string& name) : runtime_error("Error: Capacitor " + name + " already exists in the circuit\n") {}
};
class TecIndector : public runtime_error {
public:
    TecIndector(const string& name) : runtime_error("Error: Inductor " + name + " already exists in the circuit\n") {}
};
class TecVCVS : public runtime_error {
public:
    TecVCVS(const string& name) : runtime_error("Error: VCVS " + name + " already exists in the circuit.\n") {}
};
class TecVCCS : public runtime_error {
public:
    TecVCCS(const string& name) : runtime_error("Error: VCCS " + name + " already exists in the circuit.\n") {}
};
class TecCCVS : public runtime_error {
public:
    TecCCVS(const string& name) : runtime_error("Error: CCVS " + name + " already exists in the circuit.\n") {}
};
class TecCCCS : public runtime_error {
public:
    TecCCCS(const string& name) : runtime_error("Error: CCCS " + name + " already exists in the circuit.\n") {}
};
class WhereControler : public runtime_error {
public:
    WhereControler(const string& control_source_name, const string& dependent_source_name, const string& type) : runtime_error("Error: Control source " + control_source_name + " for " + type + " " + dependent_source_name + " is not a valid (non-virtual) Voltage Source.\n") {}
};
class KodoomNode : public runtime_error {
public:
    KodoomNode(const string& node_name) : runtime_error("Error: Node " + node_name + " does not exist in the circuit\n") {}
};
class SyntaxError : public runtime_error {
public:
    SyntaxError(const string& command_part = "") : runtime_error("Error: Syntax error" + (command_part.empty() ? "" : " in command: " + command_part) + "\n") {}
};
class InvalidParameter : public runtime_error {
public:
    InvalidParameter(const string& param_name = "") : runtime_error("Error: Invalid parameter" + (param_name.empty() ? "" : " (" + param_name + ")") + "\n") {}
};
class AmplitudeZero : public runtime_error {
public:
    AmplitudeZero() : runtime_error("Error: amplitude cannot be zero\n") {}
};
class FrequencyInvalid : public runtime_error {
public:
    FrequencyInvalid() : runtime_error("Error: Frequency cannot be negative or zero\n") {}
};
class PulsePeriodInvalid : public runtime_error {
public:
    PulsePeriodInvalid() : runtime_error("Error: Pulse period must be positive\n") {}
};
class TimeManfi : public runtime_error {
public:
    TimeManfi(const string& time_type) : runtime_error("Error: " + time_type + " cannot be negative\n") {}
};
class OverTperiod : public runtime_error {
public:
    OverTperiod(const string& source_type) : runtime_error("Error: Sum of Trise, Ton, and Tfall cannot exceed Tperiod\n") {}
};
class UnknownWave : public runtime_error {
public:
    UnknownWave(const string& wave_type, const string& source_name) : runtime_error("Error: Unknown wave type for " + source_name + " " + wave_type + ".\n") {}
};
class InvalidResistance : public runtime_error {
public:
    InvalidResistance() : runtime_error("Error: Invalid Resistance\n") {}
};
class ResistanceZeroOrNegative : public runtime_error {
public:
    ResistanceZeroOrNegative() : runtime_error("Error: Resistance cannot be zero or negative\n") {}
};
class InvalidVoltage : public runtime_error {
public:
    InvalidVoltage() : runtime_error("Error: Invalid Voltage\n") {}
};
class VoltageZero : public runtime_error {
public:
    VoltageZero() : runtime_error("Error: Voltage cannot be zero\n") {}
};
class InvalidCurrent : public runtime_error {
public:
    InvalidCurrent() : runtime_error("Error: Invalid Current\n") {}
};
class CurrentZero : public runtime_error {
public:
    CurrentZero() : runtime_error("Error: Current cannot be zero\n") {}
};
class InvalidCapacitance : public runtime_error {
public:
    InvalidCapacitance() : runtime_error("Error: Invalid Capacitance\n") {}
};
class CapacitanceZeroOrNegative : public runtime_error {
public:
    CapacitanceZeroOrNegative() : runtime_error("Error: Capacitance cannot be zero or negative\n") {}
};
class InvalidInitialVoltage : public runtime_error {
public:
    InvalidInitialVoltage() : runtime_error("Error: Invalid Initial Voltage\n") {}
};
class InvalidInductance : public runtime_error {
public:
    InvalidInductance() : runtime_error("Error: Invalid Inductance\n") {}
};
class InductanceZeroOrNegative : public runtime_error {
public:
    InductanceZeroOrNegative() : runtime_error("Error: Inductance cannot be zero or negative\n") {}
};
class InvalidInitialCurrent : public runtime_error {
public:
    InvalidInitialCurrent() : runtime_error("Error: Invalid Initial Current\n") {}
};
class InvalidGain : public runtime_error {
public:
    InvalidGain() : runtime_error("Error: Invalid Gain\n") {}
};
class GainZero : public runtime_error {
public:
    GainZero() : runtime_error("Error: Gain cannot be zero\n") {}
};
class InvalidResanayi : public runtime_error {
public:
    InvalidResanayi() : runtime_error("Error: Invalid Transconductance\n") {}
};
class InvalidMoghavemat : public runtime_error {
public:
    InvalidMoghavemat() : runtime_error("Error: Invalid Transresistance\n") {}
};
class NodeVirtual : public runtime_error {
public:
    NodeVirtual() : runtime_error("Error: The named node is virtual.\n") {}
};
class NodeNotGround : public runtime_error {
public:
    NodeNotGround() : runtime_error("Error: The node in question is not connected to ground.\n") {}
};
class DeleteWhatComponent : public runtime_error {
public:
    DeleteWhatComponent(const string& name) : runtime_error("Error: Cannot delete component " + name + "; component not found.\n") {}
};
class DeleteMajaziComponent : public runtime_error {
public:
    DeleteMajaziComponent(const string& name) : runtime_error("Error: Cannot delete component " + name + "; it is a virtual component.\n") {}
};
class DeleteFailed : public runtime_error {
public:
    DeleteFailed(const string& name) : runtime_error("Error: Failed to delete component " + name + " due to an internal error.\n") {}
};
class InvalidOutputFormat : public runtime_error {
public:
    InvalidOutputFormat(const string& target_s) : runtime_error("Syntax error in command: Invalid output format " + target_s + ". Expected V(name) or I(name).\n") {}
};
class OutputTargetNotFound : public runtime_error {
public:
    OutputTargetNotFound(const string& target_n, const string& type_request) : runtime_error("Error: Output target " + target_n + " not found for " + type_request + "() request.\n") {}
};
class ReadCurrentNode : public runtime_error {
public:
    ReadCurrentNode(const string& target_n) : runtime_error("Error: Cannot read current of a node. " + target_n + " is a node.\n") {}
};
class UnknownOutputType : public runtime_error {
public:
    UnknownOutputType(char type_char) : runtime_error("Syntax error in command: Unknown output type " + string(1, type_char) + ". Expected 'V' or 'I'.\n") {}
};
class NoPrintOutputs : public runtime_error {
public:
    NoPrintOutputs() : runtime_error("Syntax error in command\n") {}
};
class MovaziVS : public runtime_error {
public:
    MovaziVS() : runtime_error("Two voltage sources cannot be paralleled.\n") {}
};
class SeriCS : public runtime_error {
public:
    SeriCS() : runtime_error("Two current sources cannot be in series.\n") {}
};



const string RECENT_FILES_LIST = "recent_schematics.txt";
vector<string> CircuitFiles;
double harfadad(string s) {
    s.erase(remove(s.begin(), s.end(), ' '), s.end());

    int IsManfi = 1;
    if (!s.empty() && s[0] == '-') {
        IsManfi = -1;
        s.erase(0, 1);
    }
    double PasVand = 1.0;

    if (!s.empty()) {
        char LastChar = s.back();
        if (isalpha(LastChar)) {
            s.pop_back();

            map<char, double> PishVandes = {
                    {'T', 1e12},
                    {'G', 1e9},
                    {'M', 1e6},
                    {'k', 1e3},
                    {'m', 1e-3},
                    {'u', 1e-6},
                    {'n', 1e-9},
                    {'p', 1e-12}
            };

            if (PishVandes.count(LastChar)) {
                PasVand = PishVandes[LastChar];
            } else {
                s.push_back(LastChar);
            }
        }
    }
    double value = stod(s);
    return value * PasVand * IsManfi;
}
string JodaSazi(const string& Kalame, const string& PishVand) {
    if (Kalame.rfind(PishVand, 0) == 0) {
        return Kalame.substr(PishVand.size());
    }
    return "";
}
vector<vector<double>> copyy(const vector<vector<double>> &A) {
    vector<vector<double>> B = A;
    return B;
}
double determinant(vector<vector<double>> A) {
    int n = int(A.size());
    double det = 1.0;
    int change = 0;
    for (int i = 0; i < n; ++i) {
        int iMax = i;
        for (int j = i + 1; j < n; ++j)
            if (abs(A[j][i]) > abs(A[iMax][i]))
                iMax = j;
        if (abs(A[iMax][i]) < 1e-9)
            return 0;
        if (i != iMax) {
            swap(A[i], A[iMax]);
            change++;
        }
        det *= A[i][i];
        for (int j = i + 1; j < n; ++j) {
            double Zarib = A[j][i] / A[i][i];
            for (int k = i; k < n; ++k)
                A[j][k] -= Zarib * A[i][k];
        }
    }
    if (change % 2 != 0)
        det = -det;
    return det;
}
vector<vector<double>> replace(const vector<vector<double>> &A, const vector<double> &L, int colIndex) {
    vector<vector<double>> B = copyy(A);
    for (int i = 0; i < B.size(); ++i){B[i][colIndex] = L[i];}
    return B;
}
vector<double> Cramer(const vector<vector<double>> &A, const vector<double> &L) {
    int n = int(A.size());
    vector<double> result(n);
    double detA = determinant(A);
    if (abs(detA) < 1e-9) {
        throw MonfaredMNA();
    }
    for (int i = 0; i < n; ++i) {
        vector<vector<double>> Ai = replace(A, L, i);
        result[i] = determinant(Ai) / detA;
    }
    return result;
}

class CircuitSolver;
class Component;
class Resistor;
class VoltageSource;

class Node{
public:
    Node(const string& name) : name(name), Voltage(0.0) , IsG(false) , Andis(-1){}
    string name;
    double Voltage;
    bool IsMajaz = false;
    bool IsG;
    int Andis ;
    void setVoltage(vector<double> voltages){
        if (this->Andis != -1 && this->Andis < voltages.size()) {
            this->Voltage = voltages[this->Andis];
        }
    }
    void getVoltage(){
        cout << this->name << " voltage = " << fixed << setprecision(8) << (this->Voltage) << " volts" << endl;
    }
};
class Component{
public:
    string name;
    double Value;
    Node* Nude1;
    Node* Nude2 ;
    bool IsMajaz = false;
    virtual void getVoltage() = 0 ;
    virtual void getCurrent() = 0 ;
    virtual void resetState(){}
    virtual void updateTimeDependentValue(double t) {}
    virtual bool isVoltageSource() { return false; }
    virtual bool isCurrentSource() { return false; }
};
class Resistor : public Component{
public:
    void getVoltage() override{
        cout << this->name << " voltage = " << fixed << setprecision(8) << (this->Nude1->Voltage - this->Nude2->Voltage) << " volts" << endl;
    }
    void getCurrent() override{
        cout << this->name << " current = " << fixed << setprecision(8) << ((this->Nude1->Voltage - this->Nude2->Voltage)/this->Value) << " amps" << endl;
    }
    void resetState() override {
    }
};
class VoltageSource : public Component{
public:
    double Iv = 0.0;
    int Andis = -1;
    void getVoltage() override{
        cout << this->name << " voltage = " <<fixed << setprecision(8) << (this->Value) << " volts" << endl;
    }
    void getCurrent() override {
        cout << this->name << " current = " << fixed << setprecision(8) << (this->Iv) << " amps" << endl;
    }
    void setCurrent(double x) {
        this->Iv = x;
    }
    bool isVoltageSource() override { return true; }
    void resetState() override {
        this->Iv=0.0;
    }
};
class CurrentSource : public Component {
public:
    void getVoltage() override {
        cout << this->name << " voltage = " << fixed << setprecision(8) << (this->Nude1->Voltage - this->Nude2->Voltage) << " volts" << endl;
    }
    void getCurrent() override {
        cout << this->name << " current = " << fixed << setprecision(8) << (this->Value) << " amps" << endl;
    }
    bool isCurrentSource() override { return true; }
    void resetState() override {
    }
};
class Capacitor : public Component {
public:
    double VoltageGabl = 0.0;
    double initialVoltage = 0.0;
    Resistor* MResistor = nullptr;
    CurrentSource* MCurrentSource = nullptr;

    Capacitor(double val, const string& n, Node* node1, Node* node2, double initVolt) {
        Value = val;
        name = n;
        Nude1 = node1;
        Nude2 = node2;
        this->initialVoltage = initVolt;
        this->VoltageGabl = initVolt;
    }
    void getVoltage() override {
        cout << this->name << " voltage = " << fixed << setprecision(8)
             << (this->Nude1->Voltage - this->Nude2->Voltage) << " volts" << endl;
    }

    void getCurrent() override {
        double currentVoltage = this->Nude1->Voltage - this->Nude2->Voltage;
        double calculatedCurrent = 0.0;

        if (MResistor && MResistor->Value > 1e-12) {
            calculatedCurrent += currentVoltage / MResistor->Value;
        }

        if (MCurrentSource) {
            calculatedCurrent += MCurrentSource->Value;
        }

        cout << this->name << " current = " << fixed << setprecision(8)
             << calculatedCurrent << " amps" << endl;
    }

    void updateValues(double dt) {
        if (MResistor && MCurrentSource) {
            MResistor->Value = dt / this->Value; // R_eq = dt / C

            // I_eq = C * V_prev / dt
            MCurrentSource->Value = this->Value * this->VoltageGabl / dt;

//            MCurrentSource->Nude1 = this->Nude1;
//            MCurrentSource->Nude2 = this->Nude2;
        }
    }

    void resetState() override {
        this->VoltageGabl = this->initialVoltage;
    }
    ~Capacitor() {
        if (MResistor) {
            delete MResistor;
            MResistor = nullptr;
        }
        if (MCurrentSource) {
            delete MCurrentSource;
            MCurrentSource = nullptr;
        }
    }
};
class Inductor : public Component {
public:
    double CurrentGabl = 0.0;
    double initialCurrent = 0.0;
    Resistor* MResistor = nullptr;
    VoltageSource* MVoltageSource = nullptr;
    Node* virtualNode = nullptr;

    Inductor(double val, const string& n, Node* node1, Node* node2, double initCurr) {
        Value = val;
        name = n;
        Nude1 = node1;
        Nude2 = node2;
        this->initialCurrent = initCurr;
        this->CurrentGabl = initCurr;
    }

    void getVoltage() override {
        cout << this->name << " voltage = " << fixed << setprecision(8) << (this->Nude1->Voltage - this->Nude2->Voltage) << " volts" << endl;
    }

    void getCurrent() override {
        cout << this->name << " current = " << fixed << setprecision(8) << this->CurrentGabl << " amps" << endl;
    }

    void updateValues(double dt) {
        if (MResistor && MVoltageSource) {
            MResistor->Value = this->Value / dt;

            // V_eq = L * I_prev / dt
            MVoltageSource->Value = this->Value * this->CurrentGabl / dt;


//            MResistor->Nude1 = this->Nude1;
//            MResistor->Nude2 = this->virtualNode;
//
//            MVoltageSource->Nude1 = this->virtualNode;
//            MVoltageSource->Nude2 = this->Nude2;
        }
    }

    void resetState() override {
        this->CurrentGabl = this->initialCurrent;
    }
    ~Inductor() {
        if (MResistor) {
            delete MResistor;
            MResistor = nullptr;
        }
        if (MVoltageSource) {
            delete MVoltageSource;
            MVoltageSource = nullptr;
        }
        if (virtualNode) {
            delete virtualNode;
            virtualNode = nullptr;
        }
    }
};
class SineVoltageSource : public VoltageSource {
public:
    double Offset;
    double Damane;
    double Frequency;
    double Phase;

    SineVoltageSource(double offset, double damane, double frequency, double phase_deg , const string& n, Node* node1, Node* node2) : Offset(offset), Damane(damane), Frequency(frequency), Phase(phase_deg * M_PI / 180.0) {
        name = n;
        Nude1 = node1;
        Nude2 = node2;
        Value = Offset * sin(Phase);
    }
    void updateTimeDependentValue(double t) override {
        Value = Offset + Damane * sin(2.0 * M_PI * Frequency * t + Phase);
    }
};
class SineCurrentSource : public CurrentSource {
public:
    double Offset;
    double Damane;
    double Frequency;
    double Phase;

    SineCurrentSource(double offset, double damane, double frequency, double phase_deg,const string& n, Node* node1, Node* node2): Offset(offset), Damane(damane), Frequency(frequency), Phase(phase_deg * M_PI / 180.0) {
        name = n;
        Nude1 = node1;
        Nude2 = node2;
        Value = Offset * sin(Phase);
    }
    void updateTimeDependentValue(double t) override {
        Value = Offset + Damane * sin(2.0 * M_PI * Frequency * t + Phase);
    }
};
class PulseVoltageSource : public VoltageSource {
public:
    double VDown;
    double VUp;
    double TPeriod;
    double TRise;
    double TFall;
    double TOn;

    PulseVoltageSource(double vdown, double vup, double tperiod, double trise, double tfall, double ton, const string& n, Node* node1, Node* node2)
            : VDown(vdown), VUp(vup), TPeriod(tperiod), TRise(trise), TFall(tfall), TOn(ton) {
        name = n;
        Nude1 = node1;
        Nude2 = node2;
        Value = vdown;
    }

    void updateTimeDependentValue(double t) override {
        if (TPeriod <= 0) {
            Value = VDown;
            return;
        }

        double tP = fmod(t, TPeriod);

        if (tP >= 0 && tP < TRise) {
            Value = VDown + (VUp - VDown) * (tP / TRise);
        }
        else if (tP >= TRise && tP < (TRise + TOn)) {
            Value = VUp;
        }
        else if (tP >= (TRise + TOn) && tP < (TRise + TOn + TFall)) {
            Value = VUp - (VUp - VDown) * ((tP - (TRise + TOn)) / TFall);
        }
        else {
            Value = VDown;
        }
    }
};
class DeltaVoltageSource : public VoltageSource {
public:
    double TPeriod;
    double Epsilon = 1e-6;

    DeltaVoltageSource(double t_period, const string& n, Node* node1, Node* node2) : TPeriod(t_period) {
        name = n;
        Nude1 = node1;
        Nude2 = node2;
        Value = 0.0;
    }

    void updateTimeDependentValue(double t) override {
        if (Epsilon <= 0) {
            Value = 0.0;
            return;
        }
        else if(TPeriod==0){
            if (t >= 0 && t < Epsilon) {
                Value = 1.0 / Epsilon;
            } else {
                Value = 0.0;
            }
        }
        else{
            double tM = fmod(t, TPeriod);
            if (tM >= 0 && tM < Epsilon) {
                Value = 1.0 / Epsilon;
            } else {
                Value = 0.0;
            }
        }
    }
};
class PulseCurrentSource : public CurrentSource {
public:
    double IDown;
    double IUp;
    double TPeriod;
    double TRise;
    double TFall;
    double TOn;

    PulseCurrentSource(double idown, double iup, double tperiod, double trise, double tfall, double ton, const string& n, Node* node1, Node* node2) : IDown(idown), IUp(iup), TPeriod(tperiod), TRise(trise), TFall(tfall), TOn(ton) {
        name = n;
        Nude1 = node1;
        Nude2 = node2;
        Value = idown;
    }

    void updateTimeDependentValue(double t) override {
        if (TPeriod <= 0) {
            Value = IDown;
            return;
        }

        double tP = fmod(t, TPeriod);

        if (tP >= 0 && tP < TRise) {
            Value = IDown + (IUp - IDown) * (tP / TRise);
        }
        else if (tP >= TRise && tP < (TRise + TOn)) {
            Value = IUp;
        }
        else if (tP >= (TRise + TOn) && tP < (TRise + TOn + TFall)) {
            Value = IUp - (IUp - IDown) * ((tP - (TRise + TOn)) / TFall);
        }
        else {
            Value = IDown;
        }
    }
};
class DeltaCurrentSource : public CurrentSource {
public:
    double TPeriod;
    double Epsilon = 1e-6;

    DeltaCurrentSource(double t_period, const string& n, Node* node1, Node* node2) : TPeriod(t_period) {
        name = n;
        Nude1 = node1;
        Nude2 = node2;
        Value = 0.0;
    }

    void updateTimeDependentValue(double t) override {
        if (Epsilon <= 0) {
            Value = 0.0;
            return;
        }
        else if (TPeriod == 0) {
            if (t >= 0 && t < Epsilon) {
                Value = 1.0 / Epsilon;
            } else {
                Value = 0.0;
            }
        }
        else {
            double tM = fmod(t, TPeriod);
            if (tM >= 0 && tM < Epsilon) {
                Value = 1.0 / Epsilon;
            } else {
                Value = 0.0;
            }
        }
    }
};
class VCVS : public Component {
public:
    double Gain;
    Node* ControlNude1;
    Node* ControlNude2;
    double Iv = 0.0;
    int Andis = -1;

    VCVS(double gain, const string& n, Node* node1, Node* node2, Node* controlN1, Node* controlN2) {
        Gain = gain;
        name = n;
        Nude1 = node1;
        Nude2 = node2;
        ControlNude1 = controlN1;
        ControlNude2 = controlN2;
    }

    void getVoltage() override {
        cout << this->name << " voltage = " << fixed << setprecision(8) << (Gain * (ControlNude1->Voltage - ControlNude2->Voltage)) << " volts" << endl;
    }

    void getCurrent() override {
        cout << this->name << " current = " << fixed << setprecision(8) << Iv << " amps" << endl;
    }

    void setCurrent(double x) {
        this->Iv = x;
    }

    bool isVoltageSource() override { return true; }
};
class VCCS : public Component {
public:
    double GM;
    Node* ControlNude1;
    Node* ControlNude2;

    VCCS(double gm, const string& n, Node* node1, Node* node2, Node* controlN1, Node* controlN2) {
        GM = gm;
        name = n;
        Nude1 = node1;
        Nude2 = node2;
        ControlNude1 = controlN1;
        ControlNude2 = controlN2;
    }

    void getVoltage() override {
        cout << this->name << " voltage = " << fixed << setprecision(8) << (Nude1->Voltage - Nude2->Voltage) << " volts" << endl;
    }

    void getCurrent() override {
        cout << this->name << " current = " << fixed << setprecision(8) << (GM * (ControlNude1->Voltage - ControlNude2->Voltage)) << " amps" << endl;
    }

    bool isCurrentSource() override { return true; }
};
class CCVS : public Component{
public:
    double RM;
    VoltageSource* ControlVoltageSource;
    double Iv = 0.0;
    int Andis = -1;

    CCVS(double rm, const string& n, Node* node1, Node* node2, VoltageSource* controlProbe) {
        RM = rm;
        name = n;
        Nude1 = node1;
        Nude2 = node2;
        ControlVoltageSource = controlProbe;
    }

    void getVoltage() override {
        cout << this->name << " voltage = " << fixed << setprecision(8) << (RM * ControlVoltageSource->Iv) << " volts" << endl;
    }

    void getCurrent() override {
        cout << this->name << " current = " << fixed << setprecision(8) << Iv << " amps" << endl;
    }

    void setCurrent(double x) {
        this->Iv = x;
    }

    bool isVoltageSource() override { return true; }
};
class CCCS : public Component {
public:
    double Gain;
    VoltageSource* ControlVoltageSource;

    CCCS(double beta, const string& n, Node* node1, Node* node2, VoltageSource* controlProbe) {
        Gain = beta;
        name = n;
        Nude1 = node1;
        Nude2 = node2;
        ControlVoltageSource = controlProbe;
    }

    void getVoltage() override {
        cout << this->name << " voltage = " << fixed << setprecision(8) << (Nude1->Voltage - Nude2->Voltage) << " volts" << endl;
    }

    void getCurrent() override {
        cout << this->name << " current = " << fixed << setprecision(8) << (Gain * ControlVoltageSource->Iv) << " amps" << endl;
    }

    bool isCurrentSource() override { return true; }
};
class Diode : public Component {
public:
    Resistor* MResistor = nullptr;
    VoltageSource* MVoltageProbe = nullptr;
    Node* virtualDiodeNode = nullptr;
    bool IsOn = false;
    bool IsZn = false;

    Diode(const string& n, Node* node1, Node* node2) {
        name = n;
        Nude1 = node1;
        Nude2 = node2;
        Value = 0.0;
    }

    void getVoltage() override {
        cout << this->name << " voltage = " << fixed << setprecision(8) << (this->Nude1->Voltage - this->Nude2->Voltage) << " volts" << endl;
    }

    void getCurrent() override {
        if (MVoltageProbe) {
            cout << this->name << " current = " << fixed << setprecision(8) << MVoltageProbe->Iv << " amps" << endl;
        } else {
            cout << this->name << " current = " << fixed << setprecision(8) << "0.00000000 amps" << endl;
        }
    }

    void setModelResistance(double resistance) {
        if (MResistor) {
            MResistor->Value = resistance;
            if (resistance > 1e6) {
                IsOn = false;
            } else if (resistance < 1e-6) {
                IsOn = true;
            }
        }
        if(MVoltageProbe){
            if (IsOn && IsZn) {
                MVoltageProbe->Value = 0.7;
            } else {
                MVoltageProbe->Value = 0.0;
            }
        }
    }
    void resetState() override {
        IsOn = false;
    }

    ~Diode() {
        if (MResistor) {
            delete MResistor;
            MResistor = nullptr;
        }
        if (MVoltageProbe) {
            delete MVoltageProbe;
            MVoltageProbe = nullptr;
        }
        if (virtualDiodeNode) {
            delete virtualDiodeNode;
            virtualDiodeNode = nullptr;
        }
    }
};
class Circuit {
public:
    string currentfilee = "";
    vector<Node*> nodes;
    vector<Component*> components;
    double dt , Tf;
    vector<pair<char, string>> NeedPrints;
    vector<pair<char, string>> NeedPrintsDC;
    string dcSweepSourceName;
    double dcSweepStartValue = 0.0;
    double dcSweepEndValue = 0.0;
    double dcSweepStep = 0.0;
    double PishFarz = 0.0;
    Component* dcSweepSource = nullptr;
    vector<Diode*> diodesInCircuit;

    Circuit() {}

    ~Circuit() {
        for (Component* comp : components) {
            delete comp;
        }
        components.clear();

        for (Node* node : nodes) {
            delete node;
        }
        nodes.clear();
    }

    Node* addNode(const string& name) {
        Node* newNode = new Node(name);
        nodes.push_back(newNode);
        return newNode;
    }

    void addResistor(double value, const string& name, Node* n1, Node* n2) {
        Resistor* newRes = new Resistor();
        newRes->Value = value;
        newRes->name = name;
        newRes->Nude1 = n1;
        newRes->Nude2 = n2;
        components.push_back(newRes);
    }

    void addVoltageSource(double value, const string& name, Node* n1, Node* n2) {
        VoltageSource* newVS = new VoltageSource();
        newVS->Value = value;
        newVS->name = name;
        newVS->Nude1 = n1;
        newVS->Nude2 = n2;
        components.push_back(newVS);
    }

    void addCurrentSource(double value, const string& name, Node* n1, Node* n2) {
        CurrentSource* newCS = new CurrentSource();
        newCS->Value = value;
        newCS->name = name;
        newCS->Nude1 = n1;
        newCS->Nude2 = n2;
        components.push_back(newCS);
    }
    void addCapacitor(double value, const string& name, Node* n1, Node* n2, double VoltageAval) {
        Capacitor* newCap = new Capacitor(value, name, n1, n2, VoltageAval);

        Resistor* eqR = new Resistor();
        eqR->name = name + "_EQR";
        eqR->Nude1 = n1;
        eqR->Nude2 = n2;
        eqR->Value = 1e12;
        eqR->IsMajaz= true;
        components.push_back(eqR);
        newCap->MResistor = eqR;

        CurrentSource* eqI = new CurrentSource();
        eqI->name = name + "_EQI";
        eqI->Nude1 = n1;
        eqI->Nude2 = n2;
        eqI->Value = 0.0;
        eqI->IsMajaz=true;
        components.push_back(eqI);
        newCap->MCurrentSource = eqI;

        components.push_back(newCap);
    }

    void addInductor(double value, const string& name, Node* n1, Node* n2, double CurrentAval ) {
        Inductor* newInd = new Inductor(value, name, n1, n2, CurrentAval);

        Node* virtNode = addNode(name + "_VIRT_NODE");
        virtNode->IsMajaz=true;
        newInd->virtualNode = virtNode;

        Resistor* eqR = new Resistor();
        eqR->name = name + "_EQR";
        eqR->Nude1 = n1;
        eqR->Nude2 = virtNode;
        eqR->Value = 1e-12;
        eqR->IsMajaz = true;
        components.push_back(eqR);
        newInd->MResistor = eqR;

        VoltageSource* eqV = new VoltageSource();
        eqV->name = name + "_EQV";
        eqV->Nude1 = virtNode;
        eqV->Nude2 = n2;
        eqV->Value = 0.0;
        eqV->IsMajaz=true;
        components.push_back(eqV);
        newInd->MVoltageSource = eqV;

        components.push_back(newInd);
    }
    void addSineVoltageSource(double offset, double damane , double frequency, double phase_deg, const string& name, Node* n1, Node* n2) {
        SineVoltageSource* newSineVS = new SineVoltageSource(offset, damane , frequency, phase_deg, name, n1, n2);
        components.push_back(newSineVS);
    }
    void addSineCurrentSource(double offset, double damane, double frequency, double phase_deg , const string& name, Node* n1, Node* n2) {
        SineCurrentSource* newSineCS = new SineCurrentSource(offset, damane, frequency, phase_deg, name, n1, n2);
        components.push_back(newSineCS);
    }
    void addPulseVoltageSource(double vdown, double vup, double tperiod, double trise, double tfall, double ton,const string& name, Node* node1, Node* node2) {
        PulseVoltageSource* newPulseVS = new PulseVoltageSource(vdown, vup, tperiod, trise, tfall, ton, name, node1, node2);
        components.push_back(newPulseVS);
    }
    void addDeltaVoltageSource( double tperiod,const string& name, Node* node1, Node* node2) {
        DeltaVoltageSource* newDeltaVS = new DeltaVoltageSource( tperiod, name, node1, node2);
        components.push_back(newDeltaVS);
    }
    void addPulseCurrentSource(double idown, double iup, double tperiod, double trise, double tfall, double ton, const string& name, Node* node1, Node* node2) {
        PulseCurrentSource* newPulseCS = new PulseCurrentSource(idown, iup, tperiod, trise, tfall, ton, name, node1, node2);
        components.push_back(newPulseCS);
    }

    void addDeltaCurrentSource(double tperiod, const string& name, Node* node1, Node* node2) {
        DeltaCurrentSource* newDeltaCS = new DeltaCurrentSource(tperiod, name, node1, node2);
        components.push_back(newDeltaCS);
    }
    void addVCVS(double gain, const string& name, Node* n_out_plus, Node* n_out_minus, Node* n_control_plus, Node* n_control_minus) {
        VCVS* newVCVS = new VCVS(gain, name, n_out_plus, n_out_minus, n_control_plus, n_control_minus);
        components.push_back(newVCVS);
    }

    void addVCCS(double transconductance, const string& name, Node* n_out_plus, Node* n_out_minus, Node* n_control_plus, Node* n_control_minus) {
        VCCS* newVCCS = new VCCS(transconductance, name, n_out_plus, n_out_minus, n_control_plus, n_control_minus);
        components.push_back(newVCCS);
    }

    void addCCVS(double transresistance, const string& name, Node* n_out_plus, Node* n_out_minus ,VoltageSource* control_voltage_source) {
        CCVS* newCCVS = new CCVS(transresistance, name, n_out_plus, n_out_minus, control_voltage_source);
        components.push_back(newCCVS);
    }
    void addCCCS(double gain, const string& name, Node* n_out_plus, Node* n_out_minus, VoltageSource* control_voltage_source) {
        CCCS* newCCCS = new CCCS(gain, name, n_out_plus, n_out_minus, control_voltage_source);
        components.push_back(newCCCS);
    }
    void addDiode(const string& name, Node* n1, Node* n2 , bool IsZener) {
        Diode* newDiode = new Diode(name, n1, n2);
        newDiode->IsZn = IsZener;

        Node* virtDiodeNode = addNode(name + "_DIODE_VIRT_NODE");
        virtDiodeNode->IsMajaz = true;
        newDiode->virtualDiodeNode = virtDiodeNode;

        Resistor* eqR = new Resistor();
        eqR->name = name + "_MODEL_RESISTOR";
        eqR->Nude1 = n1;
        eqR->Nude2 = virtDiodeNode;
        eqR->Value = 1e9;
        eqR->IsMajaz = true;
        components.push_back(eqR);
        newDiode->MResistor = eqR;

        VoltageSource* vProbe = new VoltageSource();
        vProbe->name = name + "_VOLTAGE_PROBE";
        vProbe->Nude1 = virtDiodeNode;
        vProbe->Nude2 = n2;
        vProbe->Value = IsZener ? 0.7 : 0.0;
        vProbe->IsMajaz = true;
        components.push_back(vProbe);
        newDiode->MVoltageProbe = vProbe;

        components.push_back(newDiode);
    }
    void setGround(Node* node) {
        if (node) {
            node->IsG = true;
            node->Voltage = 0.0;
        }
    }

    Node* findNode(const string& name) const {
        for (Node* n : nodes) {
            if (n->name == name) {
                return n;
            }
        }
        return nullptr;
    }

    Component* findComponent(const string& name) const {
        for (Component* c : components) {
            if (c->name == name) {
                return c;
            }
        }
        return nullptr;
    }

    void resetState() {
        for (Node* n : nodes) {
            n->Voltage = 0.0;
            n->Andis = -1;
        }
        for (Component* c : components) {
            c->resetState();
        }
    }
    void resetCommandT() {
        dt = 0.0;
        Tf = 0.0;
        NeedPrints.clear();
    }
    void resetCommandD() {
        dcSweepSourceName = "";
        dcSweepStartValue = 0.0;
        dcSweepEndValue = 0.0;
        dcSweepStep = 0.0;
        PishFarz = 0.0;
        NeedPrintsDC.clear();
    }
    int TShakhe(const Node* Nodee) const {
        if (Nodee == nullptr) {
            return 0;
        }

        int count = 0;
        for (const auto& comp : components) {
            if (comp->Nude1 == Nodee || comp->Nude2 == Nodee) {
                if (!comp->IsMajaz) {
                    count++;
                }
            }
        }
        return count;
    }
    void listNodes()  {
        if (nodes.empty()) {
            cout << "No nodes available in the circuit." << endl;
            return;
        }
        cout << "Available nodes:" << endl;
        for (size_t i = 0; i < nodes.size();i++) {
            if(!nodes[i]->IsMajaz){
                cout << nodes[i]->name;
                if (i < nodes.size() - 1) {
                    cout << ", ";
                }
            }
        }
        cout << endl;
    }

    void listComponents(const string& TypeComp = "")  {
        vector<Component*> SComps;

        if (TypeComp.empty()) {
            for(auto i : components){
                if(!i->IsMajaz){SComps.push_back(i);}
            }
        }
        else {
            char type_PishVand = TypeComp.empty() ? ' ' : TypeComp.at(0);

            for (Component* comp : components) {
                if (comp->IsMajaz) { continue; }

                bool is_match = false;
                if (TypeComp == "R") {
                    if (dynamic_cast<Resistor*>(comp)) is_match = true;
                } else if (TypeComp == "C") {
                    if (dynamic_cast<Capacitor*>(comp)) is_match = true;
                } else if (TypeComp == "L") {
                    if (dynamic_cast<Inductor*>(comp)) is_match = true;
                } else if (TypeComp == "V" || TypeComp == "I") {
                    if ((type_PishVand == 'V' && comp->name.rfind("V", 0) == 0 && comp->name.rfind("VoltageSource", 0) != 0) ||
                        (type_PishVand == 'I' && comp->name.rfind("I", 0) == 0 && comp->name.rfind("CurrentSource", 0) != 0)) {
                        is_match = true;
                    }
                } else if (TypeComp == "VoltageSource") {
                    if (comp->name.rfind("VoltageSource", 0) == 0 && dynamic_cast<VoltageSource*>(comp) && !dynamic_cast<SineVoltageSource*>(comp)) is_match = true;
                } else if (TypeComp == "CurrentSource") {
                    if (comp->name.rfind("CurrentSource", 0) == 0 && dynamic_cast<CurrentSource*>(comp) && !dynamic_cast<SineCurrentSource*>(comp)) is_match = true;
                } else if (TypeComp == "E") {
                    if (dynamic_cast<VCVS*>(comp)) is_match = true;
                } else if (TypeComp == "G") {
                    if (dynamic_cast<VCCS*>(comp)) is_match = true;
                } else if (TypeComp == "H") {
                    if (dynamic_cast<CCVS*>(comp)) is_match = true;
                } else if (TypeComp == "F") {
                    if (dynamic_cast<CCCS*>(comp)) is_match = true;
                } else if(TypeComp == "D"){
                    if(Diode* d = dynamic_cast<Diode*>(comp)){
                        if (!d->IsZn) is_match = true;
                    }
                } else if(TypeComp == "Z"){
                    if(Diode* z = dynamic_cast<Diode*>(comp)){
                        if (z->IsZn) is_match = true;
                    }
                }
                if (is_match) {
                    SComps.push_back(comp);
                }
            }
        }

        if (SComps.empty()) {
            cout << "No components of type " << (TypeComp.empty() ? "any" : TypeComp) << " found in the circuit." << endl;
            return;
        }

        cout << "Available " << (TypeComp.empty() ? "components" : TypeComp + " components") << ":" << endl;
        for (size_t i = 0; i < SComps.size(); ++i) {
            cout << SComps[i]->name;
            if (i < SComps.size() - 1) {
                cout << ", ";
            }
        }
        cout << endl;
    }


    bool renameNode(const string& OldName, const string& NewName)  {
        Node* targetNode = findNode(OldName);
        if (targetNode == nullptr) {
            throw KodoomNode(OldName);
        }

        Node* existingNodeWithNewName = findNode(NewName);
        if (existingNodeWithNewName != nullptr) {
            cout << "Error: Node name " << NewName << " already exists" << endl;
            return false;
        }

        targetNode->name = NewName;
        cout << "Node " << OldName << " renamed to " << NewName << "." << endl;
        return true;
    }
    void deleteComponentPointer(Component* Compee) {
        if (Compee == nullptr) return;

        for (int i = 0; i < int(components.size()); i++) {
            if (components[i] == Compee) {
                delete components[i];
                components.erase(components.begin() + i);
                break;
            }
        }
    }

    void deleteNode(Node* Nodee) {
        if (Nodee == nullptr) return;

        if (Nodee->IsG) {
            cout << "Warning: The removed node was connected to ground. Make sure at least one other node is connected to ground." << endl;
            Nodee->IsG = false;
        }

        for (int i = 0; i < int(nodes.size()); i++) {
            if (nodes[i] == Nodee) {
                delete nodes[i];
                nodes.erase(nodes.begin() + i);
                break;
            }
        }
    }

    void RemoveIfUnused(Node* node) {
        if (node == nullptr) return;
        int connectedBranches = TShakhe(node);
        if (connectedBranches == 0) {
            deleteNode(node);
            cout << "Removed unused node: " << node->name << endl;
        }
    }
    bool deleteResistor(const string& namee) {
        Component* Compee = findComponent(namee);
        if (Compee == nullptr || dynamic_cast<Resistor*>(Compee) == nullptr || Compee->IsMajaz) {
            return false;
        }
        Node* n1 = Compee->Nude1;
        Node* n2 = Compee->Nude2;
        deleteComponentPointer(Compee);
        RemoveIfUnused(n1);
        RemoveIfUnused(n2);
        return true;
    }
    bool deleteVoltageSource(const string& name) {
        Component* Compee = findComponent(name);
        if (Compee == nullptr || dynamic_cast<VoltageSource*>(Compee) == nullptr || Compee->IsMajaz) {
            return false;
        }
        Node* n1 = Compee->Nude1;
        Node* n2 = Compee->Nude2;
        deleteComponentPointer(Compee);
        RemoveIfUnused(n1);
        RemoveIfUnused(n2);
        return true;
    }
    bool deleteCurrentSource(const string& name) {
        Component* Compee = findComponent(name);
        if (Compee == nullptr || dynamic_cast<CurrentSource*>(Compee) == nullptr || Compee->IsMajaz) {
            return false;
        }
        Node* n1 = Compee->Nude1;
        Node* n2 = Compee->Nude2;
        deleteComponentPointer(Compee);
        RemoveIfUnused(n1);
        RemoveIfUnused(n2);
        return true;
    }
    bool deleteCapacitor(const string& name) {
        Component* Compee = findComponent(name);
        Capacitor* cap = dynamic_cast<Capacitor*>(Compee);
        if (cap == nullptr || cap->IsMajaz) {
            return false;
        }
        Node* n1 = cap->Nude1;
        Node* n2 = cap->Nude2;

        Resistor* CapMResi = cap->MResistor;
        CurrentSource* CapMCurr = cap->MCurrentSource;
        deleteComponentPointer(cap);
        for (int i = 0; i < int(components.size()); ++i) {
            if (components[i] == CapMResi) {
                components.erase(components.begin() + i);
                break;
            }
        }
        for (int i = 0; i < int(components.size()); ++i) {
            if (components[i] == CapMCurr) {
                components.erase(components.begin() + i);
                break;
            }
        }
        RemoveIfUnused(n1);
        RemoveIfUnused(n2);
        return true;
    }

    bool deleteInductor(const string& name) {
        Component* Compee = findComponent(name);
        Inductor* ind = dynamic_cast<Inductor*>(Compee);
        if (ind == nullptr || ind->IsMajaz) {
            return false;
        }
        Node* n1 = ind->Nude1;
        Node* n2 = ind->Nude2;
        Node* virtNode = ind->virtualNode;

        Resistor* IndMResi = ind->MResistor;
        VoltageSource* IndMVolt = ind->MVoltageSource;
        Node* IndVNode = ind->virtualNode;
        deleteComponentPointer(ind);

        for (int i = 0; i < static_cast<int>(components.size()); ++i) {
            if (components[i] == IndMResi) {
                components.erase(components.begin() + i);
                break;
            }
        }
        for (int i = 0; i < static_cast<int>(components.size()); ++i) {
            if (components[i] == IndMVolt) {
                components.erase(components.begin() + i);
                break;
            }
        }
        if (IndVNode != nullptr) {
            for (int i = 0; i < static_cast<int>(nodes.size()); ++i) {
                if (nodes[i] == IndVNode) {
                    nodes.erase(nodes.begin() + i);
                    break;
                }
            }
        }
        RemoveIfUnused(n1);
        RemoveIfUnused(n2);
        return true;
    }
    bool deleteVCVS(const string& name) {
        Component* targetComp = findComponent(name);
        VCVS* vcvs = dynamic_cast<VCVS*>(targetComp);
        if (vcvs == nullptr || vcvs->IsMajaz) {
            return false;
        }
        Node* n1 = vcvs->Nude1;
        Node* n2 = vcvs->Nude2;
        deleteComponentPointer(vcvs);
        RemoveIfUnused(n1);
        RemoveIfUnused(n2);
        return true;
    }
    bool deleteVCCS(const string& name) {
        Component* targetComp = findComponent(name);
        VCCS* vccs = dynamic_cast<VCCS*>(targetComp);
        if (vccs == nullptr || vccs->IsMajaz) {
            return false;
        }

        Node* n1 = vccs->Nude1;
        Node* n2 = vccs->Nude2;

        deleteComponentPointer(vccs);

        RemoveIfUnused(n1);
        RemoveIfUnused(n2);

        return true;
    }

    bool deleteCCVS(const string& name) {
        Component* targetComp = findComponent(name);
        CCVS* ccvs = dynamic_cast<CCVS*>(targetComp);
        if (ccvs == nullptr || ccvs->IsMajaz) {
            return false;
        }
        Node* n1 = ccvs->Nude1;
        Node* n2 = ccvs->Nude2;

        deleteComponentPointer(ccvs);
        RemoveIfUnused(n1);
        RemoveIfUnused(n2);
        return true;
    }

    bool deleteCCCS(const string& name) {
        Component* targetComp = findComponent(name);
        CCCS* cccs = dynamic_cast<CCCS*>(targetComp);
        if (cccs == nullptr || cccs->IsMajaz) {
            return false;
        }
        Node* n1 = cccs->Nude1;
        Node* n2 = cccs->Nude2;

        deleteComponentPointer(cccs);
        RemoveIfUnused(n1);
        RemoveIfUnused(n2);
        return true;
    }
    bool deleteDiode(const string& name) {
        Component* Compee = findComponent(name);
        Diode* diode = dynamic_cast<Diode*>(Compee);
        if (diode == nullptr || diode->IsMajaz) {
            return false;
        }
        Node* n1 = diode->Nude1;
        Node* n2 = diode->Nude2;
        Node* virtDiodeNode = diode->virtualDiodeNode;

        Resistor* diodeMResistor = diode->MResistor;
        VoltageSource* diodeMVoltageProbe = diode->MVoltageProbe;

        deleteComponentPointer(diode);
        for (int i = 0; i < int(components.size()); ++i) {
            if (components[i] == diodeMResistor) {
                components.erase(components.begin() + i);
                break;
            }
        }
        for (int i = 0; i < int(components.size()); ++i) {
            if (components[i] == diodeMVoltageProbe) {
                components.erase(components.begin() + i);
                break;
            }
        }
        if (virtDiodeNode != nullptr) {
            for (int i = 0; i < static_cast<int>(nodes.size()); ++i) {
                if (nodes[i] == virtDiodeNode) {
                    nodes.erase(nodes.begin() + i);
                    break;
                }
            }
        }
        RemoveIfUnused(n1);
        RemoveIfUnused(n2);
        return true;
    }
    void clearCircuit() {
        for (Component* comp : components) {
            delete comp;
        }
        components.clear();
        for (Node* node : nodes) {
            delete node;
        }
        nodes.clear();
        resetCommandT();
        resetCommandD();
        currentfilee = "";
    }
    bool SolveAC();
    bool SolveDC_Sweep();
    bool SolveDiodes(CircuitSolver& solverD, size_t diodeAndis) ;
};
class CircuitSolver {
private:
    vector<Node*> stored_unknowns;
    vector<VoltageSource*> stored_voltageSources;
    vector<VCVS*> stored_vcvs;
    vector<CCVS*> stored_ccvs;
    bool isInitialized = false;

    void TayinGrand(Node*& chosenGroundNode, vector<Node*>& unknowns_param , Circuit& cir) {
        chosenGroundNode = nullptr;
        int Zamins = 0;
        for (Node* n : cir.nodes) {
            if (n->IsG) {
                Zamins++;
                n->Voltage = 0.0;
                if (!chosenGroundNode) {
                    chosenGroundNode = n;
                }
            }
        }
        if(Zamins > 1){
            cout << "Warning: Your circuit has multiple grounds." << endl;
        }
        if (!chosenGroundNode && !cir.nodes.empty()) {
            cout << "Error:" << "You did not select any node as ground." /*" The negative terminal of the first voltage source you put in the circuit is automatically assumed to be ground"*/ << endl;
            return;
//            for (Component* c : cir.components) {
//                if (auto* v = dynamic_cast<VoltageSource*>(c)) {
//                    if (!v->Nude2->IsG) {
//                        v->Nude2->IsG = true;
//                        v->Nude2->Voltage = 0.0;
//                        chosenGroundNode = v->Nude2;
//                    }
//                    break;
//                }
//            }
//            if (!chosenGroundNode) {
//                chosenGroundNode = cir.nodes[0];
//                chosenGroundNode->IsG = true;
//                chosenGroundNode->Voltage = 0.0;
//            }
        }
        if (!chosenGroundNode) {
            return;
        }
        unknowns_param.clear();
        for (Node* n : cir.nodes) {
            if (!n->IsG) {
                unknowns_param.push_back(n);
            }
        }
    }
    void CreateMNA(int U, int M_indep, int M_vcvs, int M_ccvs, Node* chosenGroundNode,const vector<Node*>& unknowns,const vector<VoltageSource*>& voltageSources, const vector<VCVS*>& vcvs_sources, const vector<CCVS*>& ccvs_sources, vector<vector<double>>& MNA, vector<double>& RHS,Circuit& cir) {
        for (int i = 0; i < U; ++i) {
            unknowns[i]->Andis = i;
        }
        for (int k = 0; k < M_indep; ++k) {
            voltageSources[k]->Andis = U + k;
        }
        for (int k = 0; k < M_vcvs; ++k) {
            vcvs_sources[k]->Andis = U + M_indep + k;
        }
        for (int k = 0; k < M_ccvs; ++k) {
            ccvs_sources[k]->Andis = U + M_indep + M_vcvs + k;
        }
// 5. پر کردن ماتریس G (معادلات KCL برای نودهای ناشناخته) و بردار I (منابع جریان و سهم مقاومت‌ها)
        for (Component* c : cir.components) {
            if (auto* r = dynamic_cast<Resistor*>(c)) {
                Node *n1 = r->Nude1, *n2 = r->Nude2;
                double g = 1.0 / r->Value;
                int idx1 = n1->Andis;
                int idx2 = n2->Andis;
                if (!n1->IsG) {
                    MNA[idx1][idx1] += g;
                    if (!n2->IsG) {
                        MNA[idx1][idx2] -= g;
                    } else {
                        RHS[idx1] += g * n2->Voltage;
                    }
                }
                if (!n2->IsG) {
                    MNA[idx2][idx2] += g;
                    if (!n1->IsG) {
                        MNA[idx2][idx1] -= g;
                    } else {
                        RHS[idx2] += g * n1->Voltage;
                    }
                }
            } else if (auto* curr = dynamic_cast<CurrentSource*>(c)) {
                Node *n1 = curr->Nude1, *n2 = curr->Nude2;
                double val = curr->Value;
                if (!n1->IsG) RHS[n1->Andis] += val;
                if (!n2->IsG) RHS[n2->Andis] -= val;
            }
            else if (auto* vcvs_comp = dynamic_cast<VCVS*>(c)) {
                int idx1 = vcvs_comp->Nude1->Andis;
                int idx2 = vcvs_comp->Nude2->Andis;
                int vcvs_m_idx = vcvs_comp->Andis;
                if (!vcvs_comp->Nude1->IsG) {
                    MNA[idx1][vcvs_m_idx] += 1;
                }
                if (!vcvs_comp->Nude2->IsG) {
                    MNA[idx2][vcvs_m_idx] -= 1;
                }
            }
            else if (auto* vccs_comp = dynamic_cast<VCCS*>(c)) {
                Node *n1 = vccs_comp->Nude1;
                Node *n2 = vccs_comp->Nude2;
                Node *nc1 = vccs_comp->ControlNude1;
                Node *nc2 = vccs_comp->ControlNude2;
                double gm = vccs_comp->GM;

                if (!n1->IsG) {
                    if (!nc1->IsG) MNA[n1->Andis][nc1->Andis] -= gm;
                    else RHS[n1->Andis] += gm * nc1->Voltage;
                    if (!nc2->IsG) MNA[n1->Andis][nc2->Andis] += gm;
                    else RHS[n1->Andis] -= gm * nc2->Voltage;
                }
                if (!n2->IsG) {
                    if (!nc1->IsG) MNA[n2->Andis][nc1->Andis] += gm;
                    else RHS[n2->Andis] -= gm * nc1->Voltage;
                    if (!nc2->IsG) MNA[n2->Andis][nc2->Andis] -= gm;
                    else RHS[n2->Andis] += gm * nc2->Voltage;
                }
            }
            else if (auto* ccvs_comp = dynamic_cast<CCVS*>(c)) {
                int idx1 = ccvs_comp->Nude1->Andis;
                int idx2 = ccvs_comp->Nude2->Andis;
                int ccvs_m_idx = ccvs_comp->Andis;
                if (!ccvs_comp->Nude1->IsG) {
                    MNA[idx1][ccvs_m_idx] += 1;
                }
                if (!ccvs_comp->Nude2->IsG) {
                    MNA[idx2][ccvs_m_idx] -= 1;
                }
            }
            else if (auto* cccs_comp = dynamic_cast<CCCS*>(c)) {
                Node *n1 = cccs_comp->Nude1;
                Node *n2 = cccs_comp->Nude2;
                VoltageSource* control_vs = cccs_comp->ControlVoltageSource;
                double beta = cccs_comp->Gain;

                int control_vs_current_idx = control_vs->Andis;

                if (!n1->IsG) {
                    MNA[n1->Andis][control_vs_current_idx] -= beta;
                }
                if (!n2->IsG) {
                    MNA[n2->Andis][control_vs_current_idx] += beta;
                }
            }
        }
// 6. پر کردن ماتریس‌های B و C و بردار E برای منابع ولتاژ
        for (int k = 0; k < M_indep; ++k) {
            VoltageSource* vs = voltageSources[k];
            double voltageValue = vs->Value;
            Node* n1 = vs->Nude1;
            Node* n2 = vs->Nude2;

            // بررسی خطای منبع ولتاژ بین دو گره Ground
            if (n1->IsG && n2->IsG) {
                throw VoltageSourceBeinDoG(vs->name);
            }

            int idx1 = n1->Andis;
            int idx2 = n2->Andis;
            int m_col_idx = vs->Andis;

            if (!n1->IsG) {
                MNA[idx1][m_col_idx] += 1; //************************************************************************************************
            }
            if (!n2->IsG) {
                MNA[idx2][m_col_idx] -= 1; //************************************************************************************************
            }
            if (!n1->IsG) {
                MNA[m_col_idx][idx1] += 1;
            } else {
                RHS[m_col_idx] -= n1->Voltage;
            }
            if (!n2->IsG) {
                MNA[m_col_idx][idx2] -= 1;
            } else {
                RHS[m_col_idx] += n2->Voltage;
            }
            RHS[m_col_idx] += voltageValue;
        }
        for (int k = 0; k < M_vcvs; ++k) {
            VCVS* vcvs_comp = vcvs_sources[k];
            Node* n1 = vcvs_comp->Nude1;
            Node* n2 = vcvs_comp->Nude2;
            Node* nc1 = vcvs_comp->ControlNude1;
            Node* nc2 = vcvs_comp->ControlNude2;
            double gain = vcvs_comp->Gain;

            int m_col_idx = vcvs_comp->Andis;

            if (!n1->IsG) {
                MNA[n1->Andis][m_col_idx] += 1; //*******************************************************************************************
            }
            if (!n2->IsG) {
                MNA[n2->Andis][m_col_idx] -= 1; //*******************************************************************************************
            }

            // معادله ولتاژ (بخش C و D)
            if (!n1->IsG) {
                MNA[m_col_idx][n1->Andis] += 1;
            } else {
                RHS[m_col_idx] -= n1->Voltage;
            }
            if (!n2->IsG) {
                MNA[m_col_idx][n2->Andis] -= 1;
            } else {
                RHS[m_col_idx] += n2->Voltage;
            }

            // ترم‌های کنترلی (بخش C)
            if (!nc1->IsG) {
                MNA[m_col_idx][nc1->Andis] -= gain;
            } else {
                RHS[m_col_idx] += gain * nc1->Voltage;
            }
            if (!nc2->IsG) {
                MNA[m_col_idx][nc2->Andis] += gain;
            } else {
                RHS[m_col_idx] -= gain * nc2->Voltage;
            }
        }
        for (int k = 0; k < M_ccvs; ++k) {
            CCVS* ccvs_comp = ccvs_sources[k];
            Node* n1 = ccvs_comp->Nude1;
            Node* n2 = ccvs_comp->Nude2;
            VoltageSource* control_vs = ccvs_comp->ControlVoltageSource;
            double rm = ccvs_comp->RM;

            int m_col_idx = ccvs_comp->Andis;
            int control_vs_current_idx = control_vs->Andis;

            // ترم‌های KCL (بخش B)
            if (!n1->IsG) {
                MNA[n1->Andis][m_col_idx] += 1; //***************************************************************************************************
            }
            if (!n2->IsG) {
                MNA[n2->Andis][m_col_idx] -= 1; //***************************************************************************************************
            }

            // معادله ولتاژ (بخش C و D)
            if (!n1->IsG) {
                MNA[m_col_idx][n1->Andis] += 1;
            } else {
                RHS[m_col_idx] -= n1->Voltage;
            }
            if (!n2->IsG) {
                MNA[m_col_idx][n2->Andis] -= 1;
            } else {
                RHS[m_col_idx] += n2->Voltage;
            }

            // ترم‌های کنترلی (بخش D)
            MNA[m_col_idx][control_vs_current_idx] -= rm;
        }
    }
    void UpdateCircuitint (int U, int M_indep, int M_vcvs, int M_ccvs, const vector<double>& total,const vector<Node*>& unknowns,const vector<VoltageSource*>& voltageSources , const vector<VCVS*>& vcvs_sources, const vector<CCVS*>& ccvs_sources, Circuit& cir) {
// 8. به‌روزرسانی ولتاژ نودها و جریان منابع ولتاژ
        for (Node* n : cir.nodes) {
            if (!n->IsG) { // فقط نودهای ناشناخته (که در unknowns هستند) را به روز می‌کنیم
                if (n->Andis != -1 && n->Andis < total.size()) {
                    n->Voltage = total[n->Andis];
                }
            }
// نودهای زمین (IsG=true) ولتاژ 0.0 خود را حفظ می‌کنند
        }
        int current_M_idx = U;
        for (int k = 0; k < M_indep; ++k) {
            if (voltageSources[k]->Andis < total.size()) {
                voltageSources[k]->setCurrent(total[voltageSources[k]->Andis]);
            } else {
                voltageSources[k]->setCurrent(0.0);
            }
        }

        for (int k = 0; k < M_vcvs; ++k) {
            if (vcvs_sources[k]->Andis < total.size()) {
                vcvs_sources[k]->setCurrent(total[vcvs_sources[k]->Andis]);
            } else {
                vcvs_sources[k]->setCurrent(0.0);
            }
        }

        for (int k = 0; k < M_ccvs; ++k) {
            if (ccvs_sources[k]->Andis < total.size()) {
                ccvs_sources[k]->setCurrent(total[ccvs_sources[k]->Andis]);
            } else {
                ccvs_sources[k]->setCurrent(0.0);
            }
        }
    }
public:
    bool initializeCircuitStructure(Circuit& cir) {
        Node* chosenGroundNode = nullptr;

        TayinGrand(chosenGroundNode, stored_unknowns, cir);

        if (!chosenGroundNode) {
            throw NoGround();
        }
        for (size_t i = 0; i < stored_voltageSources.size(); ++i) {
            VoltageSource* v1 = stored_voltageSources[i];
            for (size_t j = i + 1; j < stored_voltageSources.size(); ++j) {
                VoltageSource* v2 = stored_voltageSources[j];

                bool parallel_direct = (v1->Nude1 == v2->Nude1 && v1->Nude2 == v2->Nude2);
                bool parallel_reverse = (v1->Nude1 == v2->Nude2 && v1->Nude2 == v2->Nude1);

                if (parallel_direct || parallel_reverse) {
                    if (abs(v1->Value - v2->Value) > 1e-9) {
                        throw MovaziVS();
                    }
                }
            }
        }
        for (Node* n : cir.nodes) {
            if (n->IsG) continue;

            vector<CurrentSource*> connectedCurrentSources;
            for (Component* c : cir.components) {
                CurrentSource* cs = dynamic_cast<CurrentSource*>(c);
                if (cs && !cs->IsMajaz) {
                    if (cs->Nude1 == n || cs->Nude2 == n) {
                        connectedCurrentSources.push_back(cs);
                    }
                }
            }
            if (connectedCurrentSources.size() == 2 && cir.TShakhe(n) == 2) {
                CurrentSource* cs1 = connectedCurrentSources[0];
                CurrentSource* cs2 = connectedCurrentSources[1];

                if (abs(cs1->Value - cs2->Value) > 1e-9) {
                    throw SeriCS();
                }
            }
        }
        stored_voltageSources.clear();
        stored_vcvs.clear();
        stored_ccvs.clear();
        for (Component* c : cir.components) {
            if (c->isVoltageSource()) {
                if (VCVS* vcvs_comp = dynamic_cast<VCVS*>(c)) {
                    stored_vcvs.push_back(vcvs_comp);
                } else if (CCVS* ccvs_comp = dynamic_cast<CCVS*>(c)) {
                    stored_ccvs.push_back(ccvs_comp);
                }
                else{
                    stored_voltageSources.push_back(dynamic_cast<VoltageSource*>(c));
                }
            }
        }
        isInitialized = true;
        return true;
    }

    bool SolveCircuit(Circuit& cir) {
        Node* chosenGroundNode = nullptr;

        if (!isInitialized) {
            if (!initializeCircuitStructure(cir)) {
                throw AndisGozariFailed();
            }
        }


        vector<Node*>& unknowns_ref = stored_unknowns;
        vector<VoltageSource*>& voltageSources_ref = stored_voltageSources;
        vector<VCVS*>& vcvs_ref = stored_vcvs;
        vector<CCVS*>& ccvs_ref = stored_ccvs;

        int U = unknowns_ref.size();
        int M_indep = voltageSources_ref.size();
        int M_vcvs = vcvs_ref.size();
        int M_ccvs = ccvs_ref.size();
        int size = U + M_indep + M_vcvs + M_ccvs;

        if (size == 0) {
            return true;
        }
        vector<vector<double>> MNA(size, vector<double>(size, 0.0));
        vector<double> RHS(size, 0.0);
        try {
            this->CreateMNA(U, M_indep, M_vcvs, M_ccvs, chosenGroundNode, unknowns_ref, voltageSources_ref, vcvs_ref, ccvs_ref, MNA, RHS, cir);
        } catch (const runtime_error& e) {
            throw; // Re-throw the specific exception
        }
        vector<double> total = Cramer(MNA, RHS);
        if (total.empty()) {
            return false;
        }
        this->UpdateCircuitint(U, M_indep, M_vcvs, M_ccvs, total, unknowns_ref, voltageSources_ref, vcvs_ref, ccvs_ref, cir);
        return true;
    }
};
bool Circuit::SolveDiodes(CircuitSolver& solverD, size_t diodeAndis) {
    if (diodeAndis == diodesInCircuit.size()) {
        if (!solverD.SolveCircuit(*this)) {
            return false;
        }
        for (auto d : diodesInCircuit) {
            double VDiode = d->Nude1->Voltage - d->Nude2->Voltage;
            if (!d->IsOn) {
                if(!d->IsZn){
                    if (VDiode > 1e-6) {
                        return false;
                    }
                }
                else{
                    if (VDiode > 0.7 + 1e-6) {
                        return false;
                    }
                }
            }
            else {
                if (d->MVoltageProbe->Iv < -1e-9) {
                    return false;
                }
                if (d->IsZn) {
                    if (abs(VDiode - 0.7) > 1e-4) {
                        return false;
                    }
                } else {
                    if (abs(VDiode - 0.0) > 1e-4) {
                        return false;
                    }
                }
            }
        }
        return true;
    }
    Diode* DiodeX = diodesInCircuit[diodeAndis];

    DiodeX->setModelResistance(1e-9);
    DiodeX->IsOn = true;
    if (SolveDiodes(solverD, diodeAndis + 1)) {
        return true;
    }
    DiodeX->setModelResistance(1e9);
    DiodeX->IsOn = false;
    if (SolveDiodes(solverD, diodeAndis + 1)) {
        return true;
    }
    return false;
}
bool Circuit::SolveAC() {
    if (dt <= 0) {
        throw InvalidTimeStep();
    }
    if (Tf < 0) {
        throw ManfiTimeKol();
    }

    cout << "*** Starting AC (Transient) Simulation ***" << endl;
    cout << "Total Simulation Time: " << fixed << setprecision(8) << Tf << " s" << endl;
    cout << "Time Step (dt): " << fixed << setprecision(8) << dt << " s" << endl;

    resetState();

    CircuitSolver solver;
    if (!solver.initializeCircuitStructure(*this)) {
        throw AndisGozariFailed();
    }
    diodesInCircuit.clear();
    for (Component* comp : components) {
        if (Diode* d = dynamic_cast<Diode*>(comp)) {
            diodesInCircuit.push_back(d);
        }
    }
    for (Component* comp : components) {
        if(DeltaVoltageSource* delt = dynamic_cast<DeltaVoltageSource*>(comp)){
            delt->Epsilon = dt;
        }
    }
    for (double tt = 0; tt <= Tf + dt / 2.0; tt += dt) {
        for (Component* comp : components) {
            if (Capacitor* cap = dynamic_cast<Capacitor*>(comp)) {
                cap->updateValues(dt);
            } else if (Inductor* ind = dynamic_cast<Inductor*>(comp)) {
                ind->updateValues(dt);
            }
            comp->updateTimeDependentValue(tt);
        }
        if (!SolveDiodes(solver,0)) {
            throw SolveCircuitFailed(tt);
        }

        if (!NeedPrints.empty()) {
            cout << "t= " << fixed << setprecision(8) << tt  << "(s) :"<< endl;
            for (const auto& output_pair : NeedPrints) {
                char type_char = output_pair.first;
                const string& TargetN = output_pair.second;

                if (type_char == 'V') {
                    Node* targetNode = findNode(TargetN);
                    Component* targetComponent = findComponent(TargetN);

                    if (targetNode) {
                        targetNode->getVoltage();
                    }
                    else if (targetComponent) {
                        targetComponent->getVoltage();
                    }
                    else {
                        cout << "N/A" << endl;
                    }
                }
                else if (type_char == 'I') {
                    Component* targetComponent = findComponent(TargetN);

                    if (targetComponent) {
                        targetComponent->getCurrent();
                    } else {
                        cout << "N/A" << endl;
                    }
                }
            }
        }

        for (Component* comp : components) {
            if (Capacitor* cap = dynamic_cast<Capacitor*>(comp)) {
                cap->VoltageGabl = cap->Nude1->Voltage - cap->Nude2->Voltage;
            } else if (Inductor* ind = dynamic_cast<Inductor*>(comp)) {
//                ind->CurrentGabl = ind->MVoltageSource->Iv; // Inductor current is the current of its equivalent voltage source
                ind->CurrentGabl = (ind->MResistor->Nude1->Voltage - ind->MResistor->Nude2->Voltage) / ind->MResistor->Value;
            }
        }
    }

    cout << "*** AC (Transient) Simulation Finished ***" << endl;
    resetCommandT();
    return true;
}
bool Circuit::SolveDC_Sweep() {
    if (dcSweepSource == nullptr) {
        throw InvalidDCSweep();
    }
    if (dcSweepStep == 0) {
        throw TimeStepDCZero();
    }
    if (NeedPrintsDC.empty()) {
        throw NoOutDCSweep();
    }

    cout << "*** Starting DC Sweep Simulation ***" << endl;
    cout << "Sweeping Source: " << dcSweepSourceName << " from " << fixed << setprecision(8) << dcSweepStartValue << " to " << dcSweepEndValue << " with increment " << dcSweepStep << endl;

    VoltageSource* vsSweep = dynamic_cast<VoltageSource*>(dcSweepSource);
    CurrentSource* csSweep = dynamic_cast<CurrentSource*>(dcSweepSource);
    CircuitSolver solver;
    if (!solver.initializeCircuitStructure(*this)) {
        cout << "Error: Circuit structure initialization failed for DC Sweep." << endl;
        resetCommandD();
        return false;
    }

    diodesInCircuit.clear();
    for (Component* comp : components) {
        if (Diode* d = dynamic_cast<Diode*>(comp)) {
            diodesInCircuit.push_back(d);
        }
    }

    for (Component* comp : components) {
        if (Capacitor* i = dynamic_cast<Capacitor*>(comp)) {
            i->MResistor->Value = 1e12;
            i->MCurrentSource->Value = 0.0;
        } else if (Inductor* i = dynamic_cast<Inductor*>(comp)) {
            i->MResistor->Value = 1e-12;
            i->MVoltageSource->Value = 0.0;
        }
        if (SineVoltageSource* i = dynamic_cast<SineVoltageSource*>(comp)) {
            i->Value = i->Offset;
        } else if (PulseVoltageSource* i = dynamic_cast<PulseVoltageSource*>(comp)) {
            i->Value = i->VDown;
        } else if (DeltaVoltageSource* i = dynamic_cast<DeltaVoltageSource*>(comp)) {
            i->Value = 0.0;
        }
        if (SineCurrentSource* i = dynamic_cast<SineCurrentSource*>(comp)) {
            i->Value = i->Offset;
        }else if (PulseCurrentSource* i = dynamic_cast<PulseCurrentSource*>(comp)) {
            i->Value = i->IDown;
        } else if (DeltaCurrentSource* i = dynamic_cast<DeltaCurrentSource*>(comp)) {
            i->Value = 0.0;
        }
    }

    double sweep_value = dcSweepStartValue;
    bool go_up = (dcSweepStep > 0);

    while (true) {
        if (vsSweep) {
            vsSweep->Value = sweep_value;
            cout << dcSweepSourceName  << "= " << fixed << setprecision(8) << sweep_value  << "(volts) :"<< endl;
        }
        else if (csSweep) {
            csSweep->Value = sweep_value;
            cout << dcSweepSourceName  << "= " << fixed << setprecision(8) << sweep_value  << "(amps) :"<< endl;
        }

        resetState();

        if (!SolveDiodes(solver,0)) {
            cout << "Error: Circuit solution failed at DC Sweep value " << fixed << setprecision(8) << sweep_value << " for source " << dcSweepSourceName << endl;
            resetCommandD();
            return false;
        }

        for (const auto& i : NeedPrintsDC) {
            char type_char = i.first;
            const string& target_name = i.second;

            if (type_char == 'V') {
                Node* targetNode = findNode(target_name);
                Component* targetComponent = findComponent(target_name);
                if (targetNode) {
                    targetNode->getVoltage();
                } else if (targetComponent) {
                    targetComponent->getVoltage();
                } else { cout << "N/A" << endl; }
            }
            else if (type_char == 'I') {
                Component* targetComponent = findComponent(target_name);
                if (targetComponent) {
                    targetComponent->getCurrent();
                }
                else { cout << "N/A" << endl; }
            }
        }

        if (go_up) {
            if (sweep_value + dcSweepStep > dcSweepEndValue + dcSweepStep / 2.0) break;
        } else {
            if (sweep_value + dcSweepStep < dcSweepEndValue + dcSweepStep / 2.0) break;
        }
        sweep_value += dcSweepStep;
    }

    cout << "*** DC Sweep Simulation Finished ***" << endl;
    if (vsSweep) vsSweep->Value = PishFarz;
    else if (csSweep) csSweep->Value = PishFarz;
    resetCommandD();
    dcSweepSource = nullptr;
    return true;
}
class Manage {
public:
    string khar;
    Manage(const string& khat) : khar(khat) {}

    void add(Circuit& cir) {
        regex addR(R"(^\s*add\s+)");
        regex deleteR(R"(^\s*delete\s+)");
        regex readR(R"(^\s*read\s+)");
        regex currentR(R"(\s+current\s+)");
        regex voltageR(R"(\s+voltage\s+)");
        regex gndR(R"(^\s*GND\s*$)");
        regex nodesR(R"(^\s*\.nodes\s*$)");
        regex listR(R"(^\s*\.list\s*)");
        regex renameNodeR(R"(^\s*\.rename\s+node\s+)");
        if (regex_search(this->khar, addR)) {
            istringstream kalam(this->khar);
            string kalame;
            kalam >> kalame;

            string namee;
            if (!(kalam >> namee)) {
                throw SyntaxError();
            }
            char TypeComponent = namee.at(0);

            string node1Name, node2Name;
            Node *Nudee1 = nullptr;
            Node *Nudee2 = nullptr;
            double Valuee = 0.0;
            double AvalieValue = 0.0;
            string SValue;
            if (TypeComponent == 'V' && namee.rfind("VoltageSource", 0) != 0) {

                if (cir.findComponent(namee) != nullptr) {
                    throw TecVS(namee);
                }
                if (!(kalam >> node1Name)) {
                    throw SyntaxError();
                }
                Nudee1 = cir.findNode(node1Name);
                if (Nudee1 == nullptr) { Nudee1 = cir.addNode(node1Name); }
                if (!(kalam >> node2Name)) {
                    throw SyntaxError();
                }
                Nudee2 = cir.findNode(node2Name);
                if (Nudee2 == nullptr) { Nudee2 = cir.addNode(node2Name); }

                kalam >> ws;

                char next_char_wave_type = kalam.peek();
                string TypeSignal;

                if (next_char_wave_type == 'S' || next_char_wave_type == 'P' || next_char_wave_type == 'D') {
                    getline(kalam, TypeSignal, '(');
                    TypeSignal.erase(remove(TypeSignal.begin(), TypeSignal.end(), ' '), TypeSignal.end());

                    if (TypeSignal == "SIN") {
                        string params_in_paren;
                        getline(kalam, params_in_paren, ')');

                        stringstream ParamSS(params_in_paren);

                        double offset, damane, frequency, phase_deg = 0.0;
                        string ParamStr;

                        if (!(ParamSS >> ParamStr)) {
                            throw SyntaxError();
                        }
                        try { offset = harfadad(ParamStr); }
                        catch (const invalid_argument &e) {
                            throw InvalidParameter("Offset");
                        }
                        catch (const out_of_range &e) {
                            throw InvalidParameter("Offset");
                        }

                        if (!(ParamSS >> ParamStr)) {
                            throw SyntaxError();
                        }
                        try { damane = harfadad(ParamStr); }
                        catch (const invalid_argument &e) {
                            throw InvalidParameter("Amplitude");
                        }
                        catch (const out_of_range &e) {
                            throw InvalidParameter("Amplitude");
                        }
                        if (fabs(damane) < 1e-12) {
                            throw AmplitudeZero();
                        }

                        if (!(ParamSS >> ParamStr)) {
                            throw SyntaxError();
                        }
                        try { frequency = harfadad(ParamStr); }
                        catch (const invalid_argument &e) {
                            throw InvalidParameter("Frequency");
                        }
                        catch (const out_of_range &e) {
                            throw InvalidParameter("Frequency");
                        }
                        if (frequency < 1e-12) {
                            throw FrequencyInvalid();
                        }

                        if (ParamSS >> ParamStr) {
                            try { phase_deg = harfadad(ParamStr); }
                            catch (const invalid_argument &e) {
                                throw InvalidParameter("Phase");
                            }
                            catch (const out_of_range &e) {
                                throw InvalidParameter("Phase");
                            }

                            string TahKhat;
                            if (ParamSS >> TahKhat) {
                                cout << "Warning: Extra unparsed text found after command: " << TahKhat
                                     << "...'. Command processed up to valid syntax." << endl;
                            }
                        }

                        cir.addSineVoltageSource(offset, damane, frequency, phase_deg, namee, Nudee1, Nudee2);
                        cout << "Added SIN Voltage Source: " << namee << endl;

                    } else if (TypeSignal == "PULSE") {
                        string params_in_paren;
                        getline(kalam, params_in_paren, ')');
                        stringstream ParamSS(params_in_paren);

                        double v_up, t_period, t_rise, t_fall, t_on;
                        string ParamStr;

                        if (!(ParamSS >> ParamStr)) {
                            throw SyntaxError();
                        }
                        try { v_up = harfadad(ParamStr); }
                        catch (const invalid_argument &e) {
                            throw InvalidParameter("VUp");
                        }
                        catch (const out_of_range &e) {
                            throw InvalidParameter("VUp");
                        }

                        if (!(ParamSS >> ParamStr)) {
                            throw SyntaxError();
                        }
                        try { t_period = harfadad(ParamStr); }
                        catch (const invalid_argument &e) {
                            throw InvalidParameter("TPeriod");
                        }
                        catch (const out_of_range &e) {
                            throw InvalidParameter("TPeriod");
                        }
                        if (t_period < 1e-12) {
                            throw PulsePeriodInvalid();
                        }

                        if (!(ParamSS >> ParamStr)) {
                            throw SyntaxError();
                        }
                        try { t_rise = harfadad(ParamStr); }
                        catch (const invalid_argument &e) {
                            throw InvalidParameter("TRise");
                        }
                        catch (const out_of_range &e) {
                            throw InvalidParameter("TRise");
                        }
                        if (t_rise < 0) {
                            throw TimeManfi("Rise time");
                        }

                        if (!(ParamSS >> ParamStr)) {
                            throw SyntaxError();
                        }
                        try { t_fall = harfadad(ParamStr); }
                        catch (const invalid_argument &e) {
                            throw InvalidParameter("TFall");
                        }
                        catch (const out_of_range &e) {
                            throw InvalidParameter("TFall");
                        }
                        if (t_fall < 0) {
                            throw TimeManfi("Fall time");
                        }

                        if (!(ParamSS >> ParamStr)) {
                            throw SyntaxError();
                        }
                        try { t_on = harfadad(ParamStr); }
                        catch (const invalid_argument &e) {
                            throw InvalidParameter("TOn");
                        }
                        catch (const out_of_range &e) {
                            throw InvalidParameter("TOn");
                        }
                        if (t_on < 0) {
                            throw TimeManfi("Pulse width");
                        }

                        if (t_rise + t_on + t_fall > t_period) {
                            throw OverTperiod("voltage");
                        }

                        double v_down = 0.0;
                        string EzafiParan;

                        if (ParamSS >> EzafiParan) {
                            try {
                                v_down = harfadad(EzafiParan);
                            }
                            catch (const invalid_argument &e) {
                                throw InvalidParameter("Vdown");
                            }
                            catch (const out_of_range &e) {
                                throw InvalidParameter("Vdown");
                            }

                            string TahKhat;
                            if (ParamSS >> TahKhat) {
                                cout << "Warning: Extra unparsed text found after command: " << TahKhat
                                     << "...'. Command processed up to valid syntax." << endl;
                            }
                        }
                        cir.addPulseVoltageSource(v_down, v_up, t_period, t_rise, t_fall, t_on, namee, Nudee1, Nudee2);
                        cout << "Added Pulse Voltage Source: " << namee << endl;
                    } else if (TypeSignal == "DELTA") {
                        string params_in_paren;
                        getline(kalam, params_in_paren, ')');
                        stringstream ParamSS(params_in_paren);

                        double t_period = 0.0;
                        string ParamStr;

                        if (ParamSS >> ParamStr) {
                            try { t_period = harfadad(ParamStr); }
                            catch (const invalid_argument &e) {
                                throw InvalidParameter("TPeriod");
                            }
                            catch (const out_of_range &e) {
                                throw InvalidParameter("TPeriod");
                            }
                            if (t_period < 0) {
                                throw TimeManfi("Period");
                            }

                            string TahKhat;
                            if (ParamSS >> TahKhat) {
                                cout << "Warning: Extra unparsed text found after command: " << TahKhat
                                     << "...'. Command processed up to valid syntax." << endl;
                            }
                        }
                        cir.addDeltaVoltageSource(t_period, namee, Nudee1, Nudee2);
                        cout << "Added Delta Voltage Source: " << namee << endl;

                    } else {
                        throw UnknownWave(TypeSignal, "voltage source " + namee);
                    }
                }
            } else if (TypeComponent == 'I' && namee.rfind("CurrentSource", 0) != 0) {

                if (cir.findComponent(namee) != nullptr) {
                    throw TecCS(namee);
                }
                if (!(kalam >> node1Name)) {
                    throw SyntaxError();
                }
                Nudee1 = cir.findNode(node1Name);
                if (Nudee1 == nullptr) { Nudee1 = cir.addNode(node1Name); }
                if (!(kalam >> node2Name)) {
                    throw SyntaxError();
                }
                Nudee2 = cir.findNode(node2Name);
                if (Nudee2 == nullptr) { Nudee2 = cir.addNode(node2Name); }

                kalam >> ws;

                char next_char_wave_type = kalam.peek();
                string TypeSignal;

                if (next_char_wave_type == 'S' || next_char_wave_type == 'P' || next_char_wave_type == 'D') {
                    getline(kalam, TypeSignal, '(');
                    TypeSignal.erase(remove(TypeSignal.begin(), TypeSignal.end(), ' '), TypeSignal.end());

                    if (TypeSignal == "SIN") {
                        string params_in_paren;
                        getline(kalam, params_in_paren, ')');
                        stringstream ParamSS(params_in_paren);

                        double offset, damane, frequency, phase_deg = 0.0;
                        string ParamStr;

                        if (!(ParamSS >> ParamStr)) {
                            throw SyntaxError();
                        }
                        try { offset = harfadad(ParamStr); }
                        catch (const invalid_argument &e) {
                            throw InvalidParameter("Offset");
                        }
                        catch (const out_of_range &e) {
                            throw InvalidParameter("Offset");
                        }

                        if (!(ParamSS >> ParamStr)) {
                            throw SyntaxError();
                        }
                        try { damane = harfadad(ParamStr); }
                        catch (const invalid_argument &e) {
                            throw InvalidParameter("Amplitude");
                        }
                        catch (const out_of_range &e) {
                            throw InvalidParameter("Amplitude");
                        }
                        if (fabs(damane) < 1e-12) {
                            throw AmplitudeZero();
                        }


                        if (!(ParamSS >> ParamStr)) {
                            throw SyntaxError();
                        }
                        try { frequency = harfadad(ParamStr); }
                        catch (const invalid_argument &e) {
                            throw InvalidParameter("Frequency");
                        }
                        catch (const out_of_range &e) {
                            throw InvalidParameter("Frequency");
                        }
                        if (frequency < 1e-12) {
                            throw FrequencyInvalid();
                        }

                        if (ParamSS >> ParamStr) {
                            try { phase_deg = harfadad(ParamStr); }
                            catch (const invalid_argument &e) {
                                throw InvalidParameter("Phase");
                            }
                            catch (const out_of_range &e) {
                                throw InvalidParameter("Phase");
                            }

                            string TahKhat;
                            if (ParamSS >> TahKhat) {
                                cout << "Warning: Extra unparsed text found after command: " << TahKhat
                                     << "...'. Command processed up to valid syntax." << endl;
                            }
                        }

                        cir.addSineCurrentSource(offset, damane, frequency, phase_deg, namee, Nudee1, Nudee2);
                        cout << "Added SIN Current Source: " << namee << endl;

                    }
                    else if (TypeSignal == "PULSE") {
                        string params_in_paren;
                        getline(kalam, params_in_paren, ')');
                        stringstream ParamSS(params_in_paren);

                        double i_up , i_down=0.0 , t_period, t_rise, t_fall, t_on;
                        string ParamStr;

                        if (!(ParamSS >> ParamStr)) {
                            throw SyntaxError();
                        }
                        try { i_up = harfadad(ParamStr); }
                        catch (const invalid_argument &e) { throw InvalidParameter("IUp"); }
                        catch (const out_of_range &e) { throw InvalidParameter("IUp"); }

                        if (!(ParamSS >> ParamStr)) {
                            throw SyntaxError();
                        }
                        try { t_period = harfadad(ParamStr); }
                        catch (const invalid_argument &e) { throw InvalidParameter("TPeriod"); }
                        catch (const out_of_range &e) { throw InvalidParameter("TPeriod"); }
                        if (t_period < 1e-12) { throw PulsePeriodInvalid(); }

                        if (!(ParamSS >> ParamStr)) {
                            throw SyntaxError();
                        }
                        try { t_rise = harfadad(ParamStr); }
                        catch (const invalid_argument &e) { throw InvalidParameter("TRise"); }
                        catch (const out_of_range &e) { throw InvalidParameter("TRise"); }
                        if (t_rise < 0) { throw TimeManfi("Rise time"); }

                        if (!(ParamSS >> ParamStr)) {
                            throw SyntaxError();
                        }
                        try { t_fall = harfadad(ParamStr); }
                        catch (const invalid_argument &e) { throw InvalidParameter("TFall"); }
                        catch (const out_of_range &e) { throw InvalidParameter("TFall"); }
                        if (t_fall < 0) { throw TimeManfi("Fall time"); }

                        if (!(ParamSS >> ParamStr)) {
                            throw SyntaxError();
                        }
                        try { t_on = harfadad(ParamStr); }
                        catch (const invalid_argument &e) { throw InvalidParameter("TOn"); }
                        catch (const out_of_range &e) { throw InvalidParameter("TOn"); }
                        if (t_on < 0) { throw TimeManfi("Pulse width"); }

                        if (t_rise + t_on + t_fall > t_period) {
                            throw OverTperiod("current");
                        }
                        if( ParamSS >> ParamStr) {
                            try { i_down = harfadad(ParamStr); }
                            catch (const invalid_argument &e) {
                                throw InvalidParameter("IDown");
                            }
                            catch (const out_of_range &e) {
                                throw InvalidParameter("IDown");
                            }
                            string TahKhatPulseCS;
                            if (ParamSS >> TahKhatPulseCS) {
                                cout << "Warning: Extra unparsed text found after command: '" << TahKhatPulseCS << "...'. Command processed up to valid syntax." << endl;
                            }
                        }

                        cir.addPulseCurrentSource(i_down, i_up, t_period, t_rise, t_fall, t_on, namee, Nudee1, Nudee2);
                        cout << "Added PULSE Current Source: " << namee << endl;
                    }
                    else if (TypeSignal == "DELTA") {
                        string params_in_paren;
                        getline(kalam, params_in_paren, ')');
                        stringstream ParamSS(params_in_paren);

                        double t_period = 0.0;
                        string ParamStr;

                        if (ParamSS >> ParamStr) {
                            try { t_period = harfadad(ParamStr); }
                            catch (const invalid_argument &e) { throw InvalidParameter("TPeriod"); }
                            catch (const out_of_range &e) { throw InvalidParameter("TPeriod"); }
                            if (t_period < 0) {
                                throw TimeManfi("Period");
                            }

                            string TahKhatDeltaCS;
                            if (ParamSS >> TahKhatDeltaCS) {
                                cout << "Warning: Extra unparsed text found after command: '" << TahKhatDeltaCS << "...'. Command processed up to valid syntax." << endl;
                            }
                        }
                        cir.addDeltaCurrentSource(t_period, namee, Nudee1, Nudee2);
                        cout << "Added DELTA Current Source: " << namee << endl;
                    }
                    else {
                        throw UnknownWave(TypeSignal, "current source " + namee);
                    }
                }
            } else if (TypeComponent == 'R') {
                if (cir.findComponent(namee) != nullptr) {
                    throw TecResistor(namee);
                }
                if (!(kalam >> node1Name)) {
                    throw SyntaxError();
                }
                Nudee1 = cir.findNode(node1Name);
                if (Nudee1 == nullptr) {
                    Nudee1 = cir.addNode(node1Name);
                }

                if (!(kalam >> node2Name)) {
                    throw SyntaxError();
                }
                Nudee2 = cir.findNode(node2Name);
                if (Nudee2 == nullptr) {
                    Nudee2 = cir.addNode(node2Name);
                }
                if (!(kalam >> SValue)) {
                    throw SyntaxError();
                }
                try {
                    Valuee = harfadad(SValue);
                }
                catch (const invalid_argument &e) {
                    throw InvalidResistance();
                } catch (const out_of_range &e) {
                    throw InvalidResistance();
                }
                if (Valuee < 1e-12) {
                    throw ResistanceZeroOrNegative();
                }

                cir.addResistor(Valuee, namee, Nudee1, Nudee2);
                cout << "Added Resistor: " << namee << endl;
            } else if (namee.rfind("VoltageSource", 0) == 0) {
                if (cir.findComponent(namee) != nullptr) {
                    throw TecVS(namee);
                }
                if (!(kalam >> node1Name)) {
                    throw SyntaxError();
                }
                Nudee1 = cir.findNode(node1Name);
                if (Nudee1 == nullptr) {
                    Nudee1 = cir.addNode(node1Name);
                }

                if (!(kalam >> node2Name)) {
                    throw SyntaxError();
                }
                Nudee2 = cir.findNode(node2Name);
                if (Nudee2 == nullptr) {
                    Nudee2 = cir.addNode(node2Name);
                }

                if (!(kalam >> SValue)) {
                    throw SyntaxError();
                }
                try {
                    Valuee = harfadad(SValue);
                }
                catch (const invalid_argument &e) {
                    throw InvalidVoltage();
                }
                catch (const out_of_range &e) {
                    throw InvalidVoltage();
                }

                if (fabs(Valuee) < 1e-12) {
                    throw VoltageZero();
                }

                cir.addVoltageSource(Valuee, namee, Nudee1, Nudee2);
                cout << "Added Voltage Source: " << namee << endl;
            } else if (namee.rfind("CurrentSource", 0) == 0) {
                if (cir.findComponent(namee) != nullptr) {
                    throw TecCS(namee);
                }
                if (!(kalam >> node1Name)) {
                    throw SyntaxError();
                }
                Nudee1 = cir.findNode(node1Name);
                if (Nudee1 == nullptr) {
                    Nudee1 = cir.addNode(node1Name);
                }

                if (!(kalam >> node2Name)) {
                    throw SyntaxError();
                }
                Nudee2 = cir.findNode(node2Name);
                if (Nudee2 == nullptr) {
                    Nudee2 = cir.addNode(node2Name);
                }

                if (!(kalam >> SValue)) {
                    throw SyntaxError();
                }
                try {
                    Valuee = harfadad(SValue);
                }
                catch (const invalid_argument &e) {
                    throw InvalidCurrent();
                }
                catch (const out_of_range &e) {
                    throw InvalidCurrent();
                }

                if (fabs(Valuee) < 1e-12) {
                    throw CurrentZero();
                }

                cir.addCurrentSource(Valuee, namee, Nudee1, Nudee2);
                cout << "Added Current Source: " << namee << endl;
            } else if (TypeComponent == 'C') {
                if (cir.findComponent(namee) != nullptr) {
                    throw TecCapacitor(namee);
                }
                if (!(kalam >> node1Name)) {
                    throw SyntaxError();
                }
                Nudee1 = cir.findNode(node1Name);
                if (Nudee1 == nullptr) {
                    Nudee1 = cir.addNode(node1Name);
                }
                if (!(kalam >> node2Name)) {
                    throw SyntaxError();
                }
                Nudee2 = cir.findNode(node2Name);
                if (Nudee2 == nullptr) {
                    Nudee2 = cir.addNode(node2Name);
                }

                if (!(kalam >> SValue)) {
                    throw SyntaxError();
                }
                try {
                    Valuee = harfadad(SValue);
                }
                catch (const invalid_argument &e) {
                    throw InvalidCapacitance();
                }
                catch (const out_of_range &e) {
                    throw InvalidCapacitance();
                }
                if (Valuee < 1e-12) {
                    throw CapacitanceZeroOrNegative();
                }

                string SInitial;
                double InitialValue = 0.0;
                if (kalam >> SInitial) {
                    try {
                        InitialValue = harfadad(SInitial);
                    }
                    catch (const invalid_argument &e) {
                        throw InvalidInitialVoltage();
                    }
                    catch (const out_of_range &e) {
                        throw InvalidInitialVoltage();
                    }

                }

                cir.addCapacitor(Valuee, namee, Nudee1, Nudee2, InitialValue);
                cout << "Added Capacitor: " << namee << endl;
            } else if (TypeComponent == 'L') {
                if (cir.findComponent(namee) != nullptr) {
                    throw TecIndector(namee);
                }
                if (!(kalam >> node1Name)) {
                    throw SyntaxError();
                }
                Nudee1 = cir.findNode(node1Name);
                if (Nudee1 == nullptr) {
                    Nudee1 = cir.addNode(node1Name);
                }
                if (!(kalam >> node2Name)) {
                    throw SyntaxError();
                }
                Nudee2 = cir.findNode(node2Name);
                if (Nudee2 == nullptr) {
                    Nudee2 = cir.addNode(node2Name);
                }

                if (!(kalam >> SValue)) {
                    throw SyntaxError();
                }
                try {
                    Valuee = harfadad(SValue);
                }
                catch (const invalid_argument &e) {
                    throw InvalidInductance();
                }
                catch (const out_of_range &e) {
                    throw InvalidInductance();
                }
                if (Valuee < 1e-12) {
                    throw InductanceZeroOrNegative();
                }

                string SInitial;
                double InitialValue = 0.0;
                if (kalam >> SInitial) {
                    try {
                        InitialValue = harfadad(SInitial);
                    }
                    catch (const invalid_argument &e) {
                        throw InvalidInitialCurrent();
                    }
                    catch (const out_of_range &e) {
                        throw InvalidInitialCurrent();
                    }

                }

                cir.addInductor(Valuee, namee, Nudee1, Nudee2, InitialValue);
                cout << "Added Inductor: " << namee << endl;
            } else if (TypeComponent == 'E') {

                if (cir.findComponent(namee) != nullptr) {
                    throw TecVCVS(namee);
                }
                string node1Name, node2Name, node3Name, node4Name;
                Node *Nudee1 = nullptr, *Nudee2 = nullptr, *Nudee3 = nullptr, *Nudee4 = nullptr;

                if (!(kalam >> node1Name)) {
                    throw SyntaxError();
                }
                Nudee1 = cir.findNode(node1Name);
                if (Nudee1 == nullptr) { Nudee1 = cir.addNode(node1Name); }

                if (!(kalam >> node2Name)) {
                    throw SyntaxError();
                }
                Nudee2 = cir.findNode(node2Name);
                if (Nudee2 == nullptr) { Nudee2 = cir.addNode(node2Name); }

                if (!(kalam >> node3Name)) {
                    throw SyntaxError();
                }
                Nudee3 = cir.findNode(node3Name);
                if (Nudee3 == nullptr) { Nudee3 = cir.addNode(node3Name); }

                if (!(kalam >> node4Name)) {
                    throw SyntaxError();
                }
                Nudee4 = cir.findNode(node4Name);
                if (Nudee4 == nullptr) { Nudee4 = cir.addNode(node4Name); }

                string SValue;
                double Valuee;
                if (!(kalam >> SValue)) {
                    throw SyntaxError();
                }
                try { Valuee = harfadad(SValue); }
                catch (const invalid_argument &e) {
                    throw InvalidGain();
                }
                catch (const out_of_range &e) {
                    throw InvalidGain();
                }
                if (fabs(Valuee) < 1e-12) {
                    throw GainZero();
                }

                cir.addVCVS(Valuee, namee, Nudee1, Nudee2, Nudee3, Nudee4);
                cout << "Added VCVS: " << namee << endl;
            } else if (TypeComponent == 'G' && !regex_match(namee, gndR)) {

                if (cir.findComponent(namee) != nullptr) {
                    throw TecVCCS(namee);
                }
                string node1Name, node2Name, node3Name, node4Name;
                Node *Nudee1 = nullptr, *Nudee2 = nullptr, *Nudee3 = nullptr, *Nudee4 = nullptr;

                if (!(kalam >> node1Name)) {
                    throw SyntaxError();
                }
                Nudee1 = cir.findNode(node1Name);
                if (Nudee1 == nullptr) { Nudee1 = cir.addNode(node1Name); }

                if (!(kalam >> node2Name)) {
                    throw SyntaxError();
                }
                Nudee2 = cir.findNode(node2Name);
                if (Nudee2 == nullptr) { Nudee2 = cir.addNode(node2Name); }

                if (!(kalam >> node3Name)) {
                    throw SyntaxError();
                }
                Nudee3 = cir.findNode(node3Name);
                if (Nudee3 == nullptr) { Nudee3 = cir.addNode(node3Name); }

                if (!(kalam >> node4Name)) {
                    throw SyntaxError();
                }
                Nudee4 = cir.findNode(node4Name);
                if (Nudee4 == nullptr) { Nudee4 = cir.addNode(node4Name); }

                string SValue;
                double Valuee;
                if (!(kalam >> SValue)) {
                    throw SyntaxError();
                }
                try { Valuee = harfadad(SValue); }
                catch (const invalid_argument &e) {
                    throw InvalidResanayi();
                }
                catch (const out_of_range &e) {
                    throw InvalidResanayi();
                }
                if (fabs(Valuee) < 1e-12) {
                    throw GainZero();
                }

                cir.addVCCS(Valuee, namee, Nudee1, Nudee2, Nudee3, Nudee4);
                cout << "Added VCCS: " << namee << endl;
            } else if (TypeComponent == 'H') {

                if (cir.findComponent(namee) != nullptr) {
                    throw TecCCVS(namee);
                }
                string node1Name, node2Name;
                Node *Nudee1 = nullptr, *Nudee2 = nullptr;

                if (!(kalam >> node1Name)) {
                    throw SyntaxError();
                }
                Nudee1 = cir.findNode(node1Name);
                if (Nudee1 == nullptr) { Nudee1 = cir.addNode(node1Name); }

                if (!(kalam >> node2Name)) {
                    throw SyntaxError();
                }
                Nudee2 = cir.findNode(node2Name);
                if (Nudee2 == nullptr) { Nudee2 = cir.addNode(node2Name); }

                string ControlVSName;
                if (!(kalam >> ControlVSName)) {
                    throw SyntaxError();
                }
                Component *ControlC = cir.findComponent(ControlVSName);
                VoltageSource *ControlVS = dynamic_cast<VoltageSource *>(ControlC);

                if (ControlVS == nullptr || ControlVS->IsMajaz) {
                    throw WhereControler(ControlVSName, namee, "CCVS");
                }
                string SValue;
                double Valuee;
                if (!(kalam >> SValue)) {
                    throw SyntaxError();
                }
                try { Valuee = harfadad(SValue); }
                catch (const invalid_argument &e) {
                    throw InvalidMoghavemat();
                }
                catch (const out_of_range &e) {
                    throw InvalidMoghavemat();
                }
                if (fabs(Valuee) < 1e-12) {
                    throw GainZero();
                }

                cir.addCCVS(Valuee, namee, Nudee1, Nudee2, ControlVS);
                cout << "Added CCVS: " << namee << endl;
            } else if (TypeComponent == 'F') {

                if (cir.findComponent(namee) != nullptr) {
                    throw TecCCCS(namee);
                }
                string node1Name, node2Name;
                Node *Nudee1 = nullptr, *Nudee2 = nullptr;

                if (!(kalam >> node1Name)) {
                    throw SyntaxError();
                }
                Nudee1 = cir.findNode(node1Name);
                if (Nudee1 == nullptr) { Nudee1 = cir.addNode(node1Name); }

                if (!(kalam >> node2Name)) {
                    throw SyntaxError();
                }
                Nudee2 = cir.findNode(node2Name);
                if (Nudee2 == nullptr) { Nudee2 = cir.addNode(node2Name); }

                string ControlVSName;
                if (!(kalam >> ControlVSName)) {
                    throw SyntaxError();
                }

                Component *ControlC = cir.findComponent(ControlVSName);
                VoltageSource *ControlVS = dynamic_cast<VoltageSource *>(ControlC);

                if (ControlVS == nullptr || ControlVS->IsMajaz) {
                    throw WhereControler(ControlVSName, namee, "CCCS");
                }

                string SValue;
                double Valuee;
                if (!(kalam >> SValue)) {
                    throw SyntaxError();
                }
                try { Valuee = harfadad(SValue); }
                catch (const invalid_argument &e) {
                    throw InvalidGain();
                }
                catch (const out_of_range &e) {
                    throw InvalidGain();
                }
                if (fabs(Valuee) < 1e-12) {
                    throw GainZero();
                }

                cir.addCCCS(Valuee, namee, Nudee1, Nudee2, ControlVS);
                cout << "Added CCCS: " << namee << endl;
            }
            else if (TypeComponent == 'D') {
                if (cir.findComponent(namee) != nullptr) {
                    cout << "Error: Diode " << namee << " already exists in the circuit." << endl;
                    return;
                }
                if (!(kalam >> node1Name)) { cout << "Error: Syntax error" << endl; return; }
                Nudee1 = cir.findNode(node1Name); if (Nudee1 == nullptr) { Nudee1 = cir.addNode(node1Name); }
                if (!(kalam >> node2Name)) { cout << "Error: Syntax error" << endl; return; }
                Nudee2 = cir.findNode(node2Name); if (Nudee2 == nullptr) { Nudee2 = cir.addNode(node2Name); }
                cir.addDiode(namee, Nudee1, Nudee2 , false);
                cout << "Added Diode: " << namee << endl;
            }
            else if (TypeComponent == 'Z') {
                if (cir.findComponent(namee) != nullptr) {
                    cout << "Error: ZenerDiode " << namee << " already exists in the circuit." << endl;
                    return;
                }
                if (!(kalam >> node1Name)) { cout << "Error: Syntax error" << endl; return; }
                Nudee1 = cir.findNode(node1Name); if (Nudee1 == nullptr) { Nudee1 = cir.addNode(node1Name); }
                if (!(kalam >> node2Name)) { cout << "Error: Syntax error" << endl; return; }
                Nudee2 = cir.findNode(node2Name); if (Nudee2 == nullptr) { Nudee2 = cir.addNode(node2Name); }
                cir.addDiode(namee, Nudee1, Nudee2 , true);
                cout << "Added ZenerDiode: " << namee << endl;
            }
            else if (regex_match(namee, gndR)) {
                if (!(kalam >> namee)) {
                    throw SyntaxError();
                }
                Nudee1 = cir.findNode(namee);
                if (Nudee1 == nullptr) {
                    throw KodoomNode(namee);
                }
                cir.setGround(Nudee1);
                cout << "Set Node " << namee << " as Ground." << endl;
            }
            else {
                throw NotFoundComponent(namee);
            }
            string TahKhat;
            if (kalam >> TahKhat) {
                cout << "Warning: Extra unparsed text found after command: " << TahKhat
                     << "...'. Command processed up to valid syntax." << endl;
            }
        }
        else if (regex_search(this->khar, deleteR)) {
            istringstream kalam(this->khar);
            string kalame;
            kalam >> kalame;

            string namee;
            if (!(kalam >> namee)) {
                throw SyntaxError();
            }
            if (regex_match(namee, gndR)) {
                if (!(kalam >> namee)) {
                    throw SyntaxError();
                }
                Node *nono = cir.findNode(namee);
                if (nono == nullptr) {
                    throw KodoomNode(namee);
                }
                if (nono->IsMajaz) {
                    throw NodeVirtual();
                }
                if (!nono->IsG) {
                    throw NodeNotGround();
                }
                nono->IsG = false;
            } else {
                Component *CompD = cir.findComponent(namee);
                if (CompD == nullptr) {
                    throw DeleteWhatComponent(namee);
                }
                if (CompD->IsMajaz) {
                    throw DeleteMajaziComponent(namee);
                }

                bool delete_successful = false;
                char TypePishVand = namee.at(0);

                if (namee.rfind("VoltageSource", 0) == 0) {
                    delete_successful = cir.deleteVoltageSource(namee);
                } else if (namee.rfind("CurrentSource", 0) == 0) {
                    delete_successful = cir.deleteCurrentSource(namee);
                } else if (TypePishVand == 'R') {
                    delete_successful = cir.deleteResistor(namee);
                } else if (TypePishVand == 'C') {
                    delete_successful = cir.deleteCapacitor(namee);
                } else if (TypePishVand == 'L') {
                    delete_successful = cir.deleteInductor(namee);
                } else if (TypePishVand == 'V') {
                    delete_successful = cir.deleteVoltageSource(namee);
                } else if (TypePishVand == 'I') {
                    delete_successful = cir.deleteCurrentSource(namee);
                } else if (TypePishVand == 'E') {
                    delete_successful = cir.deleteVCVS(namee);
                } else if (TypePishVand == 'G') {
                    delete_successful = cir.deleteVCCS(namee);
                } else if (TypePishVand == 'H') {
                    delete_successful = cir.deleteCCVS(namee);
                } else if (TypePishVand == 'F') {
                    delete_successful = cir.deleteCCCS(namee);
                } else if (TypePishVand == 'D') {
                    delete_successful = cir.deleteDiode(namee);
                } else if (TypePishVand == 'Z') {
                    delete_successful = cir.deleteDiode(namee);
                } else {
                    throw NotFoundComponent(namee);
                }
                if (!delete_successful) {
                    throw DeleteFailed(namee);
                }
                else{
                    cout << "Deleted component: " << namee << endl;
                }

                string TahKhat;
                if (kalam >> TahKhat) {
                    cout << "Warning: Extra unparsed text found after command: " << TahKhat
                         << "...'. Command processed up to valid syntax." << endl;
                }
            }
        } else if (regex_match(this->khar, nodesR)) {
            cir.listNodes();
        } else if (regex_search(this->khar, listR)) {

            istringstream kalamL(this->khar);
            string TypeC;
            kalamL >> TypeC;
            if (kalamL >> TypeC) {
                cir.listComponents(TypeC);
            } else {
                cir.listComponents();
            }
        } else if (regex_search(this->khar, renameNodeR)) {
            istringstream kalamR(this->khar);
            string oldN, newN;
            for (int i = 0; i < 2; i++) {
                kalamR >> oldN;
            }
            if (!(kalamR >> oldN) || !(kalamR >> newN)) {
                cout << "ERROR: Invalid syntax - correct format:" << endl;
                cout << ".rename node <old_name> <new_name>" << endl;
                return;
            }
            cir.renameNode(oldN, newN);
        }
        else if (khar.rfind(".print TRAN", 0) == 0) {
            istringstream kalam(khar);
            string kalame;
            kalam >> kalame >> kalame;

            string dtS, tfS;
            if (!(kalam >> dtS) || !(kalam >> tfS)) {
                throw SyntaxError("TRAN command");
            }

            try {
                cir.dt = harfadad(dtS);
                cir.Tf = harfadad(tfS);
            }
            catch (const invalid_argument &e) {
                throw InvalidParameter("Numeric value for TRAN");
            }
            catch (const out_of_range &e) {
                throw InvalidParameter("Numeric value for TRAN");
            }
            if (cir.dt <= 0) {
                throw TimeManfi("Tstep");
            }
            if (cir.Tf < 0) {
                throw ManfiTimeKol();
            }
            if (cir.Tf < cir.dt && cir.Tf != 0) {
                cout << "Warning: Tstop is less than Tstep. Simulation might run for only one step if Tstop is not zero."<< endl;
            }

            cir.NeedPrints.clear();

            string TargetS;
            while (kalam >> TargetS) {
                if (TargetS.size() < 4 || TargetS[1] != '(' || TargetS.back() != ')') {
                    throw InvalidOutputFormat(TargetS);
                }

                char type_char = TargetS[0];
                string TargetN = TargetS.substr(2, TargetS.size() - 3);

                if (type_char == 'V') {
                    Node *targetNode = cir.findNode(TargetN);
                    Component *targetComponent = cir.findComponent(TargetN);

                    if (!targetNode && !targetComponent) {
                        throw OutputTargetNotFound(TargetN, "V");
                    }
                    cir.NeedPrints.push_back({type_char, TargetN});

                }
                else if (type_char == 'I') {
                    Node *targetNode = cir.findNode(TargetN);
                    Component *targetComponent = cir.findComponent(TargetN);

                    if (targetNode) {
                        throw ReadCurrentNode(TargetN);
                    }
                    if (!targetComponent) {
                        throw OutputTargetNotFound(TargetN, "I");
                    }
                    cir.NeedPrints.push_back({type_char, TargetN});

                } else {
                    throw UnknownOutputType(type_char);
                }
            }
            if (cir.NeedPrints.empty()) {
                throw NoPrintOutputs();
            }
            cir.SolveAC();
        }
        else if (khar.rfind(".DC", 0) == 0) {
            istringstream kalam(khar);
            string kalame;
            kalam >> kalame;

            string SourceN, ValueS, ValueE, StepDC;

            if (!(kalam >> SourceN) || !(kalam >> ValueS) || !(kalam >> ValueE) || !(kalam >> StepDC)) {
                throw SyntaxError("DC command");
            }

            Component* SourceComp = cir.findComponent(SourceN);
            if (SourceComp == nullptr) {
                throw NotFoundSDCSweep(SourceN);
            }

            VoltageSource* target_vs = dynamic_cast<VoltageSource*>(SourceComp);
            CurrentSource* target_cs = dynamic_cast<CurrentSource*>(SourceComp);

            if (target_vs == nullptr && target_cs == nullptr) {
                throw InvalidDCSweepType(SourceN);
            }
            else if ( (target_vs != nullptr && target_vs->IsMajaz) || ( target_cs != nullptr && target_cs->IsMajaz) ) {
                throw MajaziDCSweepComp(SourceN);
            }
            try {
                cir.dcSweepStartValue = harfadad(ValueS);
                cir.dcSweepEndValue = harfadad(ValueE);
                cir.dcSweepStep = harfadad(StepDC);
            }
            catch (const invalid_argument& e) {
                throw InvalidParameter("Numeric value for DC sweep");
            }
            catch (const out_of_range& e) {
                throw InvalidParameter("Numeric value for DC sweep");
            }
            if (cir.dcSweepStep == 0) {
                throw TimeStepDCZero();
            }
            if ((cir.dcSweepEndValue > cir.dcSweepStartValue && cir.dcSweepStep < 0) || (cir.dcSweepEndValue < cir.dcSweepStartValue && cir.dcSweepStep > 0)) {
                throw MatchStepDCSweep();
            }

            cir.dcSweepSourceName = SourceN;
            cir.dcSweepSource = SourceComp;
            if (VoltageSource* vs = dynamic_cast<VoltageSource*>(cir.dcSweepSource)) {
                cir.PishFarz = vs->Value;
            } else if (CurrentSource* cs = dynamic_cast<CurrentSource*>(cir.dcSweepSource)) {
                cir.PishFarz = cs->Value;
            }
            cir.NeedPrintsDC.clear();
            string TargetS;
            while (kalam >> TargetS) {
                if (TargetS.size() < 4 || TargetS[1] != '(' || TargetS.back() != ')') {
                    throw InvalidOutputFormat(TargetS);
                }

                char type_char = TargetS[0];
                string TargetN = TargetS.substr(2, TargetS.size() - 3);

                if (type_char == 'V') {
                    Node* targetNode = cir.findNode(TargetN);
                    Component* targetComponent = cir.findComponent(TargetN);
                    if (!targetNode && !targetComponent) {
                        throw OutputTargetNotFound(TargetN, "V");
                    }
                    cir.NeedPrintsDC.push_back({type_char, TargetN});
                }
                else if (type_char == 'I') {
                    Node* targetNode = cir.findNode(TargetN);
                    Component* targetComponent = cir.findComponent(TargetN);
                    if (targetNode) {
                        throw ReadCurrentNode(TargetN);
                    }
                    if (!targetComponent) {
                        throw OutputTargetNotFound(TargetN, "I");
                    }
                    cir.NeedPrintsDC.push_back({type_char, TargetN});
                }
                else {
                    throw UnknownOutputType(type_char);
                }
            }

            if (cir.NeedPrintsDC.empty()) {
                throw NoPrintOutputs();
            }
            cir.SolveDC_Sweep();
        }
    }

};
/*void read_circuit_from_file(const string& filename, Circuit& circuit) {
    ifstream fin(filename);
    if (!fin.is_open()) {
        cout << "Error: Could not open file " << filename << endl;
        return;
    }

    string line;
    vector<string> node_lines;

    while (getline(fin, line)) {
        stringstream ss(line);
        string Typee;
        ss >> Typee;
        bool IsTec = false;
        for(const auto& i : node_lines){
            if(i==line){IsTec=true;}
        }
        if (Typee == "N" && !IsTec) {
            node_lines.push_back(line);
        }
    }

    for (const string& i : node_lines) {
        stringstream ss(i);
        string Typee, node_name, isG_str, isMaj_str;
        ss >> Typee >> node_name >> isG_str >> isMaj_str;

        Node* newNode = circuit.findNode(node_name);
        if (newNode == nullptr) {
            newNode = circuit.addNode(node_name);
        }

        if (JodaSazi(isG_str, "g=") == "1") {
            newNode->IsG = true;
        }
        if (JodaSazi(isMaj_str, "m=") == "1") {
            newNode->IsMajaz = true;
        }
    }

    fin.clear();
    fin.seekg(0);

    while (getline(fin, line)) {
        stringstream ss(line);
        string Typee;
        ss >> Typee;

        string name, p1_name, p2_name, cp1_name, cp2_name, cs_name, param_str;
        double val, offset, amp, freq, phase, vdown, vup, tperiod, trise, tfall, ton, gain, rm, gm , iup , idown;

        if (Typee == "N") {
            continue;
        } else if (Typee == "R") {
            ss >> name >> param_str; p1_name = JodaSazi(param_str, "p1=");
            ss >> param_str; p2_name = JodaSazi(param_str, "p2=");
            ss >> param_str; val = harfadad(JodaSazi(param_str, "v="));
            circuit.addResistor(val, name, circuit.findNode(p1_name), circuit.findNode(p2_name));
        } else if (Typee == "V") {
            ss >> name >> param_str; p1_name = JodaSazi(param_str, "p1=");
            ss >> param_str; p2_name = JodaSazi(param_str, "p2=");
            ss >> param_str; val = harfadad(JodaSazi(param_str, "v="));
            circuit.addVoltageSource(val, name, circuit.findNode(p1_name), circuit.findNode(p2_name));
        } else if (Typee == "I") {
            ss >> name >> param_str; p1_name = JodaSazi(param_str, "p1=");
            ss >> param_str; p2_name = JodaSazi(param_str, "p2=");
            ss >> param_str; val = harfadad(JodaSazi(param_str, "v="));
            circuit.addCurrentSource(val, name, circuit.findNode(p1_name), circuit.findNode(p2_name));
        } else if (Typee == "C") {
            double initial_v = 0.0;
            ss >> name >> param_str; p1_name = JodaSazi(param_str, "p1=");
            ss >> param_str; p2_name = JodaSazi(param_str, "p2=");
            ss >> param_str; val = harfadad(JodaSazi(param_str, "c="));
            if (ss >> param_str) initial_v = harfadad(JodaSazi(param_str, "iv="));
            circuit.addCapacitor(val, name, circuit.findNode(p1_name), circuit.findNode(p2_name), initial_v);
        } else if (Typee == "L") {
            double initial_i = 0.0;
            ss >> name >> param_str; p1_name = JodaSazi(param_str, "p1=");
            ss >> param_str; p2_name = JodaSazi(param_str, "p2=");
            ss >> param_str; val = harfadad(JodaSazi(param_str, "l="));
            if (ss >> param_str) initial_i = harfadad(JodaSazi(param_str, "ic="));
            circuit.addInductor(val, name, circuit.findNode(p1_name), circuit.findNode(p2_name), initial_i);
        } else if (Typee == "VSIN") {
            ss >> name >> param_str; p1_name = JodaSazi(param_str, "p1=");
            ss >> param_str; p2_name = JodaSazi(param_str, "p2=");
            ss >> param_str; offset = harfadad(JodaSazi(param_str, "o="));
            ss >> param_str; amp = harfadad(JodaSazi(param_str, "a="));
            ss >> param_str; freq = harfadad(JodaSazi(param_str, "f="));
            ss >> param_str; phase = harfadad(JodaSazi(param_str, "ph="));
            circuit.addSineVoltageSource(offset, amp, freq, phase, name, circuit.findNode(p1_name), circuit.findNode(p2_name));
        } else if (Typee == "ISIN") {
            ss >> name >> param_str; p1_name = JodaSazi(param_str, "p1=");
            ss >> param_str; p2_name = JodaSazi(param_str, "p2=");
            ss >> param_str; offset = harfadad(JodaSazi(param_str, "o="));
            ss >> param_str; amp = harfadad(JodaSazi(param_str, "a="));
            ss >> param_str; freq = harfadad(JodaSazi(param_str, "f="));
            ss >> param_str; phase = harfadad(JodaSazi(param_str, "ph="));
            circuit.addSineCurrentSource(offset, amp, freq, phase, name, circuit.findNode(p1_name), circuit.findNode(p2_name));
        } else if (Typee == "VPULSE") {
            ss >> name >> param_str; p1_name = JodaSazi(param_str, "p1=");
            ss >> param_str; p2_name = JodaSazi(param_str, "p2=");
            ss >> param_str; vdown = harfadad(JodaSazi(param_str, "vd="));
            ss >> param_str; vup = harfadad(JodaSazi(param_str, "vu="));
            ss >> param_str; tperiod = harfadad(JodaSazi(param_str, "tp="));
            ss >> param_str; trise = harfadad(JodaSazi(param_str, "tr="));
            ss >> param_str; tfall = harfadad(JodaSazi(param_str, "tf="));
            ss >> param_str; ton = harfadad(JodaSazi(param_str, "ton="));
            circuit.addPulseVoltageSource(vdown, vup, tperiod, trise, tfall, ton, name, circuit.findNode(p1_name), circuit.findNode(p2_name));
        } else if (Typee == "VDELTA") {
            ss >> name >> param_str; p1_name = JodaSazi(param_str, "p1=");
            ss >> param_str; p2_name = JodaSazi(param_str, "p2=");
            ss >> param_str; tperiod = harfadad(JodaSazi(param_str, "tp="));
            circuit.addDeltaVoltageSource(tperiod, name, circuit.findNode(p1_name), circuit.findNode(p2_name));
        }else if (Typee == "IPULSE") {
            ss >> name >> param_str; p1_name = JodaSazi(param_str, "p1=");
            ss >> param_str; p2_name = JodaSazi(param_str, "p2=");
            ss >> param_str; idown = harfadad(JodaSazi(param_str, "id="));
            ss >> param_str; iup = harfadad(JodaSazi(param_str, "iu="));
            ss >> param_str; tperiod = harfadad(JodaSazi(param_str, "tp="));
            ss >> param_str; trise = harfadad(JodaSazi(param_str, "tr="));
            ss >> param_str; tfall = harfadad(JodaSazi(param_str, "tf="));
            ss >> param_str; ton = harfadad(JodaSazi(param_str, "ton="));
            circuit.addPulseCurrentSource(idown, iup, tperiod, trise, tfall, ton, name, circuit.findNode(p1_name), circuit.findNode(p2_name));
        } else if (Typee == "IDELTA") {
            ss >> name >> param_str; p1_name = JodaSazi(param_str, "p1=");
            ss >> param_str; p2_name = JodaSazi(param_str, "p2=");
            ss >> param_str; tperiod = harfadad(JodaSazi(param_str, "tp="));
            circuit.addDeltaCurrentSource(tperiod, name, circuit.findNode(p1_name), circuit.findNode(p2_name));
        }else if (Typee == "E") {
            ss >> name >> param_str; p1_name = JodaSazi(param_str, "p1=");
            ss >> param_str; p2_name = JodaSazi(param_str, "p2=");
            ss >> param_str; cp1_name = JodaSazi(param_str, "cp1=");
            ss >> param_str; cp2_name = JodaSazi(param_str, "cp2=");
            ss >> param_str; gain = harfadad(JodaSazi(param_str, "g="));
            circuit.addVCVS(gain, name, circuit.findNode(p1_name), circuit.findNode(p2_name), circuit.findNode(cp1_name), circuit.findNode(cp2_name));
        } else if (Typee == "G") {
            ss >> name >> param_str; p1_name = JodaSazi(param_str, "p1=");
            ss >> param_str; p2_name = JodaSazi(param_str, "p2=");
            ss >> param_str; cp1_name = JodaSazi(param_str, "cp1=");
            ss >> param_str; cp2_name = JodaSazi(param_str, "cp2=");
            ss >> param_str; gm = harfadad(JodaSazi(param_str, "gm="));
            circuit.addVCCS(gm, name, circuit.findNode(p1_name), circuit.findNode(p2_name), circuit.findNode(cp1_name), circuit.findNode(cp2_name));
        } else if (Typee == "H") {
            ss >> name >> param_str; p1_name = JodaSazi(param_str, "p1=");
            ss >> param_str; p2_name = JodaSazi(param_str, "p2=");
            ss >> param_str; cs_name = JodaSazi(param_str, "cs=");
            ss >> param_str; rm = harfadad(JodaSazi(param_str, "rm="));
            VoltageSource* control_vs = dynamic_cast<VoltageSource*>(circuit.findComponent(cs_name));
            if (!control_vs) {
                cout << "Error: Control voltage source " << cs_name << " not found for CCVS " << name << endl;
                continue;
            }
            circuit.addCCVS(rm, name, circuit.findNode(p1_name), circuit.findNode(p2_name), control_vs);
        } else if (Typee == "F") {
            ss >> name >> param_str; p1_name = JodaSazi(param_str, "p1=");
            ss >> param_str; p2_name = JodaSazi(param_str, "p2=");
            ss >> param_str; cs_name = JodaSazi(param_str, "cs=");
            ss >> param_str; gain = harfadad(JodaSazi(param_str, "b="));
            VoltageSource* control_vs = dynamic_cast<VoltageSource*>(circuit.findComponent(cs_name));
            if (!control_vs) {
                cout << "Error: Control voltage source " << cs_name << " not found for CCCS " << name << endl;
                continue;
            }
            circuit.addCCCS(gain, name, circuit.findNode(p1_name), circuit.findNode(p2_name), control_vs);
        }
        else if (Typee == "DIODE") {
            ss >> name >> param_str; p1_name = JodaSazi(param_str, "p1=");
            ss >> param_str; p2_name = JodaSazi(param_str, "p2=");
            circuit.addDiode(name, circuit.findNode(p1_name), circuit.findNode(p2_name) , false);
        }
        else if (Typee == "ZENER") {
            ss >> name >> param_str; p1_name = JodaSazi(param_str, "p1=");
            ss >> param_str; p2_name = JodaSazi(param_str, "p2=");
            circuit.addDiode(name, circuit.findNode(p1_name), circuit.findNode(p2_name) , true);
        }
    }
    fin.close();
}*/
/* void save_circuit_to_file(const string& filename, const Circuit& circuit) {
    ofstream fout(filename);
    if (!fout.is_open()) {
        cout << "Error: Could not open file for writing: " << filename << endl;
        return;
    }
    fout << fixed << setprecision(10);

    for (const auto& node : circuit.nodes) {
        if(!node->IsMajaz) fout << "N " << node->name << " g=" << (node->IsG ? "1" : "0") << " m=" << (node->IsMajaz ? "1" : "0") << endl;
    }
    for (const auto& comp : circuit.components) {
        if (comp->IsMajaz) {
            continue;
        }

        if (const Resistor* r = dynamic_cast<const Resistor*>(comp)) {
            fout << "R " << r->name << " p1=" << r->Nude1->name << " p2=" << r->Nude2->name << " v=" << r->Value << endl;
        }
        else if (const VoltageSource* vs = dynamic_cast<const VoltageSource*>(comp)) {
            if (const SineVoltageSource* svs = dynamic_cast<const SineVoltageSource*>(comp)) {
                fout << "VSIN " << svs->name << " p1=" << svs->Nude1->name << " p2=" << svs->Nude2->name
                     << " o=" << svs->Offset << " a=" << svs->Damane << " f=" << svs->Frequency << " ph=" << (svs->Phase * 180.0 / M_PI) << endl;
            } else if (const PulseVoltageSource* pvs = dynamic_cast<const PulseVoltageSource*>(comp)) {
                fout << "VPULSE " << pvs->name << " p1=" << pvs->Nude1->name << " p2=" << pvs->Nude2->name
                     << " vd=" << pvs->VDown << " vu=" << pvs->VUp << " tp=" << pvs->TPeriod << " tr=" << pvs->TRise << " tf=" << pvs->TFall << " ton=" << pvs->TOn << endl;
            } else if (const DeltaVoltageSource* dvs = dynamic_cast<const DeltaVoltageSource*>(comp)) {
                fout << "VDELTA " << dvs->name << " p1=" << dvs->Nude1->name << " p2=" << dvs->Nude2->name<< " tp=" << dvs->TPeriod << endl;
            } else {
                fout << "V " << vs->name << " p1=" << vs->Nude1->name << " p2=" << vs->Nude2->name << " v=" << vs->Value << endl;
            }
        }
        else if (const CurrentSource* cs = dynamic_cast<const CurrentSource*>(comp)) {
            if (const SineCurrentSource* scs = dynamic_cast<const SineCurrentSource*>(comp)) {
                fout << "ISIN " << scs->name << " p1=" << scs->Nude1->name << " p2=" << scs->Nude2->name
                     << " o=" << scs->Offset << " a=" << scs->Damane << " f=" << scs->Frequency << " ph=" << (scs->Phase * 180.0 / M_PI) << endl;
            }
            else if (const PulseCurrentSource* pcs = dynamic_cast<const PulseCurrentSource*>(comp)) {
                fout << "IPULSE " << pcs->name << " p1=" << pcs->Nude1->name << " p2=" << pcs->Nude2->name
                     << " id=" << pcs->IDown << " iu=" << pcs->IUp << " tp=" << pcs->TPeriod << " tr=" << pcs->TRise << " tf=" << pcs->TFall << " ton=" << pcs->TOn << endl;
            } else if (const DeltaCurrentSource* dcs = dynamic_cast<const DeltaCurrentSource*>(comp)) {
                fout << "IDELTA " << dcs->name << " p1=" << dcs->Nude1->name << " p2=" << dcs->Nude2->name << " tp=" << dcs->TPeriod << endl;
            }
            else {
                fout << "I " << cs->name << " p1=" << cs->Nude1->name << " p2=" << cs->Nude2->name << " v=" << cs->Value << endl;
            }
        }
        else if (const Capacitor* c = dynamic_cast<const Capacitor*>(comp)) {
            fout << "C " << c->name << " p1=" << c->Nude1->name << " p2=" << c->Nude2->name << " c=" << c->Value << " iv=" << c->initialVoltage << endl;
        }
        else if (const Inductor* l = dynamic_cast<const Inductor*>(comp)) {
            fout << "L " << l->name << " p1=" << l->Nude1->name << " p2=" << l->Nude2->name << " l=" << l->Value << " ic=" << l->initialCurrent << endl;
        }
        else if (const VCVS* e = dynamic_cast<const VCVS*>(comp)) {
            fout << "E " << e->name << " p1=" << e->Nude1->name << " p2=" << e->Nude2->name
                 << " cp1=" << e->ControlNude1->name << " cp2=" << e->ControlNude2->name << " g=" << e->Gain << endl;
        }
        else if (const VCCS* g = dynamic_cast<const VCCS*>(comp)) {
            fout << "G " << g->name << " p1=" << g->Nude1->name << " p2=" << g->Nude2->name
                 << " cp1=" << g->ControlNude1->name << " cp2=" << g->ControlNude2->name << " gm=" << g->GM << endl;
        }
        else if (const CCVS* h = dynamic_cast<const CCVS*>(comp)) {
            fout << "H " << h->name << " p1=" << h->Nude1->name << " p2=" << h->Nude2->name
                 << " cs=" << h->ControlVoltageSource->name << " rm=" << h->RM << endl;
        }
        else if (const CCCS* f = dynamic_cast<const CCCS*>(comp)) {
            fout << "F " << f->name << " p1=" << f->Nude1->name << " p2=" << f->Nude2->name
                 << " cs=" << f->ControlVoltageSource->name << " b=" << f->Gain << endl;
        }
        else if (const Diode* d = dynamic_cast<const Diode*>(comp)) {
            if(!d->IsZn){fout << "DIODE " << d->name << " p1=" << d->Nude1->name << " p2=" << d->Nude2->name << endl;}
            else{fout << "ZENER " << d->name << " p1=" << d->Nude1->name << " p2=" << d->Nude2->name << endl;}
        }
    }
    fout.close();
} */
void loadCircuitFiles() {
    CircuitFiles.clear();
    ifstream fin(RECENT_FILES_LIST);
    if (fin.is_open()) {
        string path;
        while (getline(fin, path)) {
            if (!path.empty()) {
                CircuitFiles.push_back(path);
            }
        }
        fin.close();
    }
}
void saveCircuitFiles() {
    vector<string> FileGhabl;
    ifstream fin(RECENT_FILES_LIST);
    string line;
    while (getline(fin, line)) {
        FileGhabl.push_back(line);
    }
    fin.close();
    for (const auto& file : CircuitFiles) {
        FileGhabl.erase(remove(FileGhabl.begin(), FileGhabl.end(), file), FileGhabl.end());
    }
    for (const auto& file : CircuitFiles) {
        FileGhabl.push_back(file);
    }
    ofstream fout(RECENT_FILES_LIST);
    if (fout.is_open()) {
        for (const auto& entry : FileGhabl) {
            fout << entry << endl;
        }
        fout.close();
    }
}
void AddCircuitFile(const string& path) {
    CircuitFiles.erase(remove_if(CircuitFiles.begin(), CircuitFiles.end(),[&](const string& p) {return p == path;}),CircuitFiles.end());
    CircuitFiles.push_back(path);
    saveCircuitFiles();
}
string getCommand(Circuit& cir) {
    string line;
    while (true) {
        cout << ">> ";
        getline(cin, line);

        if (regex_match(line, regex(R"(^\s*end\s*$)"))) {
            return "end";
        }
        if (regex_match(line, regex(R"(^\s*return\s*$)"))) {
            return "return";
        }
        return line;
    }
}
/* void CircuitMenu(Circuit& cir) {
    cout << endl << (cir.currentfilee.empty() ? "NEW" : cir.currentfilee)  << "circuit opened" << endl;
    cout << "Ready for circuit commands. Type 'return' to return to the menu and 'end' to exit." << endl;
    while (true) {
        string command = getCommand(cir);

        if (command == "end") {
            exit(0);
        }
        if (command == "return") {
            return;
        }
        try {
            Manage manage(command);
            manage.add(cir);
        } catch (const runtime_error& e) {
            cout << e.what();
        }
        if (!cir.currentfilee.empty()) {
            save_circuit_to_file(cir.currentfilee, cir);
        }
    }
}
void showSchematicsMenu(Circuit& cir) {
    cout << "\n--- Available schematics (latest) ---" << endl;

    if (CircuitFiles.empty()) {
        cout << "No schematic files have been recently opened/saved." << endl;
    }
    else {
        for (size_t i = 0; i < CircuitFiles.size(); ++i) {
            cout << (i + 1) << "- " << CircuitFiles[i] << endl;
        }
    }

    cout << "\nEnter the desired schematic number to open (or 'return' to return to the main menu):" << endl;
    string ii;
    getline(cin, ii);

    if (regex_match(ii, regex(R"(^\s*end\s*$)"))) {
        exit(0);
    }
    if (regex_match(ii, regex(R"(^\s*return\s*$)"))) {
        return;
    }

    try {
        int Adad = stoi(ii);
        if (Adad > 0 && Adad <= CircuitFiles.size()) {
            string NumberSelect = CircuitFiles[Adad - 1];

            ifstream testFile(NumberSelect);
            if (!testFile.is_open()) {
                cout << "Error: File " << NumberSelect << " not found or accessible." << endl;
                CircuitFiles.erase(CircuitFiles.begin() + Adad - 1);
                saveCircuitFiles();
                return;
            }

            cout << "\n--- File Content: " << NumberSelect << " ---" << endl;
            string line;
            int lineNumber = 1;
            while (getline(testFile, line)) {
                cout << lineNumber++ << ": " << line << endl;
            }
            testFile.close();

            cout << "\nDo you want to open this file? (yes/no)" << endl;
            string confirmation;
            getline(cin, confirmation);

            if (regex_match(confirmation, regex(R"(^\s*end\s*$)"))) {
                exit(0);
            }
            if (regex_match(confirmation, regex(R"(^\s*no\s*$)"))) {
                showSchematicsMenu(cir);
                return;
            }
            if (!regex_match(confirmation, regex(R"(^\s*yes\s*$)"))) {
                cout << "-Error : Inappropriate input" << endl;
                showSchematicsMenu(cir);
                return;
            }
            cir.clearCircuit();
            read_circuit_from_file(NumberSelect, cir);

            if (!cir.nodes.empty() || !cir.components.empty()) {
                cir.currentfilee = NumberSelect;
                CircuitMenu(cir);
            } else {
                cout << "Error: Schematic file " << NumberSelect << " is empty or invalid. A new schematic will be created in it." << endl;
                cir.currentfilee = NumberSelect;
                CircuitMenu(cir);
            }

        }
        else {
            cout << "-Error : Inappropriate input" << endl;
        }
    }
    catch (const invalid_argument& e) {
        cout << "-Error : Inappropriate input" << endl;
    }
    catch (const out_of_range& e) {
        cout << "Error: The entered number is out of range." << endl;
    }
}
void openOrCreateFile(Circuit& cir) {
    cout << "\n--- Open/Create New File ---" << endl;
    cout << R"(Please enter the full file address (including name and format, example: C:\Users\User\circuit.txt))" << endl;
    cout << "Type 'return' to return to the menu and 'end' to exit." << endl;

    string filee;
    getline(cin, filee);

    if (regex_match(filee, regex(R"(^\s*end\s*$)"))) {
        exit(0);
    }
    if (regex_match(filee, regex(R"(^\s*return\s*$)"))) {
        return;
    }

    if (filee.empty()) {
        cout << "Error: File address cannot be empty." << endl;
        return;
    }
    cir.clearCircuit();

    ifstream testFile(filee);
    if (testFile.is_open()) {
        testFile.close();
        cout << "File " << filee << " found. Loading..." << endl;
        read_circuit_from_file(filee, cir);

        if (!cir.nodes.empty() || !cir.components.empty()) {
            cir.currentfilee = filee;
            AddCircuitFile(filee);
            CircuitMenu(cir);
        }
        else {
            cout << "The file " << filee << " is empty or invalid. A new schematic will be created in it." << endl;
            cir.currentfilee = filee;
            AddCircuitFile(filee);
            CircuitMenu(cir);
        }
    }
    else {
        cout << "File " << filee << " not found. Creating new file..." << endl;
        ofstream newFile(filee);
        if (newFile.is_open()) {
            newFile.close();
            cout << "New file " << filee << " created successfully." << endl;
            cir.currentfilee = filee;
            AddCircuitFile(filee);
            CircuitMenu(cir);
        }
        else {
            cout << "Error: Unable to create file " << filee << ". Invalid path/filename." << endl;
            return;
        }
    }
} */

/* void mainMenu(Circuit& cir) {
    loadCircuitFiles();
    while (true) {
        cout << "\n--- main menu ---" << endl;
        cout << "1-show existing schematics" << endl;
        cout << "2-NewFil" << endl;
        cout << "Type 'end' to exit." << endl;

        string Adad;
        getline(cin, Adad);

        if (regex_match(Adad, regex(R"(^\s*end\s*$)"))) {
            exit(0);
        }

        if (Adad == "1") {
            showSchematicsMenu(cir);
        }
        else if (Adad == "2") {
            openOrCreateFile(cir);
        } else {
            cout << "-Error : Inappropriate input" << endl;
        }
    }
} */


// ================================================

// ==== ADD: shared circuit instance for the UI ====
Circuit gCircuit;  // if you already have a Circuit variable, you can reuse it and skip this

// UI coordinates for a placed element
struct UiPos { int x, y, w, h; };
std::unordered_map<std::string, UiPos> gUiPos;

// careful: relies on Circuit storing raw pointers like in your saver
static void ClearCircuit(Circuit& cir) {
    for (auto* c : cir.components) delete c;
    cir.components.clear();
    for (auto* n : cir.nodes) delete n;
    cir.nodes.clear();
}

static void ResetModelsAndUI() {
    ClearCircuit(gCircuit);
    gUiPos.clear();
}
// ==== ADD: auto-namers for nodes and elements ====
static int gNodeCounter = 1;
static int gRID=1, gCID=1, gLID=1, gDID=1, gZID=1;
static int gVID=1, gIID=1, gEID=1, gGID=1, gHID=1, gFID=1, gVCTRLID=1;

// === Resets and cleaning ===
static void ResetCounters() {
    gNodeCounter = 1;
    gRID=1; gCID=1; gLID=1; gDID=1; gZID=1;
    gVID=1; gIID=1; gEID=1; gGID=1; gHID=1; gFID=1; gVCTRLID=1;
}

inline void ui_set_element_position(const std::string& name, int x, int y, int w, int h) {
    gUiPos[name] = {x, y, w, h};
}
// make sure these are visible somewhere (as you used before)
extern void ui_set_element_position(const std::string& name, int x, int y, int w, int h);

// fetch a component's rect; returns false if we don't have one yet
inline bool ui_get_element_rect(const std::string& name, int& x, int& y, int& w, int& h) {
    auto it = gUiPos.find(name);
    if (it == gUiPos.end()) return false;
    x = it->second.x; y = it->second.y; w = it->second.w; h = it->second.h;
    return true;
}

// clear all cached UI rects (call this when switching/closing projects)
inline void ui_clear_positions() {
    gUiPos.clear();
}



// ─────────────────────────────────────────────────────────────────────────────
// READ
// ─────────────────────────────────────────────────────────────────────────────
void read_circuit_from_file(const std::string& filename, Circuit& circuit, bool clearFirst) {
    if (clearFirst) {
        ResetModelsAndUI();
        ResetCounters();
    }

    // Keep global path in sync with what we actually opened.
    // This ensures the window title / subsequent saves follow the real file.
    extern std::string gProjectPath;
    gProjectPath = filename;

    std::ifstream fin(filename);
    if (!fin.is_open()) {
        std::cout << "Error: Could not open file " << filename << std::endl;
        return;
    }

    auto ensure_node = [&](const std::string& n)->Node* {
        if (n.empty()) return nullptr;
        if (auto* p = circuit.findNode(n)) return p;
        return circuit.addNode(n);
    };

    // ── PASS 0 (optional): skip/scan metadata/header lines
    // We allow lines like:
    //   # PROJECT name=MyProject
    // or:
    //   PROJECT name=MyProject
    // These are ignored by old readers, and harmless.
    {
        std::streampos orig = fin.tellg();
        std::string line;
        while (std::getline(fin, line)) {
            std::string t;
            std::stringstream ss(line);
            ss >> t;
            if (t == "#" || t == "PROJECT" || t == "#PROJECT") {
                // ignore; if you want to cache display name, you can parse name=... here
                continue;
            }
            // First non-metadata line reached; rewind to its start.
            fin.seekg(orig);
            break;
        }
        // If file entirely metadata (unlikely), rewind anyway.
        fin.clear();
        fin.seekg(0);
    }

    // ── PASS 1: collect unique nodes & merge flags
    std::unordered_map<std::string, std::pair<bool,bool>> nodeFlags; // name -> (IsG, IsMajaz)
    {
        fin.clear(); fin.seekg(0);
        std::string line;
        while (std::getline(fin, line)) {
            std::stringstream ss(line);
            std::string t; ss >> t;

            // ignore metadata/header/comment lines
            if (t == "#" || t == "PROJECT" || t == "#PROJECT") continue;

            if (t != "N") continue;

            std::string name, gtok, mtok;
            ss >> name >> gtok >> mtok;
            auto &flags = nodeFlags[name];
            if (JodaSazi(gtok,"g=") == "1") flags.first  = true;
            if (JodaSazi(mtok,"m=") == "1") flags.second = true;
        }
    }
    // create/update nodes
    for (const auto& kv : nodeFlags) {
        Node* n = ensure_node(kv.first);
        if (!n) continue;
        n->IsG     = kv.second.first;
        n->IsMajaz = kv.second.second;
    }

    // ── PASS 2: components
    fin.clear(); fin.seekg(0);
    std::string line;
    while (std::getline(fin, line)) {
        std::stringstream ss(line);
        std::string Typee; ss >> Typee;

        // ignore metadata/header/comment lines
        if (Typee == "#" || Typee == "PROJECT" || Typee == "#PROJECT") continue;

        if (Typee == "N" || Typee.empty()) continue;

        std::string name, p1_name, p2_name, cp1_name, cp2_name, cs_name, tok;
        double val=0, offset=0, amp=0, freq=0, phase=0;
        double vdown=0, vup=0, tperiod=0, trise=0, tfall=0, ton=0;
        double gain=0, rm=0, gm=0, iup=0, idown=0;

        // Helper to scan UI rect at end of the line (applies to main component only)
        auto apply_ui_rect = [&](const std::string& compName){
            int x=-1,y=-1,w=-1,h=-1;
            std::stringstream tail(line);
            std::string t;
            while (tail >> t) {
                if      (t.rfind("x=",0)==0) x = std::stoi(JodaSazi(t,"x="));
                else if (t.rfind("y=",0)==0) y = std::stoi(JodaSazi(t,"y="));
                else if (t.rfind("w=",0)==0) w = std::stoi(JodaSazi(t,"w="));
                else if (t.rfind("h=",0)==0) h = std::stoi(JodaSazi(t,"h="));
            }
            if (x!=-1 && y!=-1) {
                if (w<=0) w = 100;
                if (h<=0) h = 44;
                ui_set_element_position(compName, x, y, w, h);
            }
        };

        // ── PASSIVE & INDEPENDENT SOURCES
        if (Typee == "R") {
            ss >> name >> tok; p1_name = JodaSazi(tok,"p1=");
            ss >> tok;         p2_name = JodaSazi(tok,"p2=");
            ss >> tok;         val     = harfadad(JodaSazi(tok,"v="));
            circuit.addResistor(val, name, ensure_node(p1_name), ensure_node(p2_name));
            apply_ui_rect(name);
        }
        else if (Typee == "V") {
            ss >> name >> tok; p1_name = JodaSazi(tok,"p1=");
            ss >> tok;         p2_name = JodaSazi(tok,"p2=");
            ss >> tok;         val     = harfadad(JodaSazi(tok,"v="));
            circuit.addVoltageSource(val, name, ensure_node(p1_name), ensure_node(p2_name));
            apply_ui_rect(name);
        }
        else if (Typee == "I") {
            ss >> name >> tok; p1_name = JodaSazi(tok,"p1=");
            ss >> tok;         p2_name = JodaSazi(tok,"p2=");
            ss >> tok;         val     = harfadad(JodaSazi(tok,"v="));
            circuit.addCurrentSource(val, name, ensure_node(p1_name), ensure_node(p2_name));
            apply_ui_rect(name);
        }
        else if (Typee == "C") {
            double iv=0.0;
            ss >> name >> tok; p1_name = JodaSazi(tok,"p1=");
            ss >> tok;         p2_name = JodaSazi(tok,"p2=");
            ss >> tok;         val     = harfadad(JodaSazi(tok,"c="));
            if (ss >> tok) {
                if (tok.rfind("iv=",0)==0) iv = harfadad(JodaSazi(tok,"iv="));
            }
            circuit.addCapacitor(val, name, ensure_node(p1_name), ensure_node(p2_name), iv);
            apply_ui_rect(name);
        }
        else if (Typee == "L") {
            double ic=0.0;
            ss >> name >> tok; p1_name = JodaSazi(tok,"p1=");
            ss >> tok;         p2_name = JodaSazi(tok,"p2=");
            ss >> tok;         val     = harfadad(JodaSazi(tok,"l="));
            if (ss >> tok) {
                if (tok.rfind("ic=",0)==0) ic = harfadad(JodaSazi(tok,"ic="));
            }
            circuit.addInductor(val, name, ensure_node(p1_name), ensure_node(p2_name), ic);
            apply_ui_rect(name);
        }
        else if (Typee == "VSIN") {
            ss >> name >> tok; p1_name = JodaSazi(tok,"p1=");
            ss >> tok;         p2_name = JodaSazi(tok,"p2=");
            ss >> tok;         offset  = harfadad(JodaSazi(tok,"o="));
            ss >> tok;         amp     = harfadad(JodaSazi(tok,"a="));
            ss >> tok;         freq    = harfadad(JodaSazi(tok,"f="));
            ss >> tok;         phase   = harfadad(JodaSazi(tok,"ph="));
            circuit.addSineVoltageSource(offset, amp, freq, phase, name, ensure_node(p1_name), ensure_node(p2_name));
            apply_ui_rect(name);
        }
        else if (Typee == "ISIN") {
            ss >> name >> tok; p1_name = JodaSazi(tok,"p1=");
            ss >> tok;         p2_name = JodaSazi(tok,"p2=");
            ss >> tok;         offset  = harfadad(JodaSazi(tok,"o="));
            ss >> tok;         amp     = harfadad(JodaSazi(tok,"a="));
            ss >> tok;         freq    = harfadad(JodaSazi(tok,"f="));
            ss >> tok;         phase   = harfadad(JodaSazi(tok,"ph="));
            circuit.addSineCurrentSource(offset, amp, freq, phase, name, ensure_node(p1_name), ensure_node(p2_name));
            apply_ui_rect(name);
        }
        else if (Typee == "VPULSE") {
            ss >> name >> tok; p1_name = JodaSazi(tok,"p1=");
            ss >> tok;         p2_name = JodaSazi(tok,"p2=");
            ss >> tok;         vdown   = harfadad(JodaSazi(tok,"vd="));
            ss >> tok;         vup     = harfadad(JodaSazi(tok,"vu="));
            ss >> tok;         tperiod = harfadad(JodaSazi(tok,"tp="));
            ss >> tok;         trise   = harfadad(JodaSazi(tok,"tr="));
            ss >> tok;         tfall   = harfadad(JodaSazi(tok,"tf="));
            ss >> tok;         ton     = harfadad(JodaSazi(tok,"on="));
            if (ton == 0 && tok.rfind("ton=",0)==0) ton = harfadad(JodaSazi(tok,"ton="));
            if (ton == 0) { std::stringstream scan(line); std::string t2; while (scan >> t2) if (t2.rfind("ton=",0)==0) { ton = harfadad(JodaSazi(t2,"ton=")); break; } }
            circuit.addPulseVoltageSource(vdown, vup, tperiod, trise, tfall, ton, name, ensure_node(p1_name), ensure_node(p2_name));
            apply_ui_rect(name);
        }
        else if (Typee == "VDELTA") {
            ss >> name >> tok; p1_name = JodaSazi(tok,"p1=");
            ss >> tok;         p2_name = JodaSazi(tok,"p2=");
            ss >> tok;         tperiod = harfadad(JodaSazi(tok,"tp="));
            circuit.addDeltaVoltageSource(tperiod, name, ensure_node(p1_name), ensure_node(p2_name));
            apply_ui_rect(name);
        }
        else if (Typee == "IPULSE") {
            ss >> name >> tok; p1_name = JodaSazi(tok,"p1=");
            ss >> tok;         p2_name = JodaSazi(tok,"p2=");
            ss >> tok;         idown   = harfadad(JodaSazi(tok,"id="));
            ss >> tok;         iup     = harfadad(JodaSazi(tok,"iu="));
            ss >> tok;         tperiod = harfadad(JodaSazi(tok,"tp="));
            ss >> tok;         trise   = harfadad(JodaSazi(tok,"tr="));
            ss >> tok;         tfall   = harfadad(JodaSazi(tok,"tf="));
            ss >> tok;         ton     = harfadad(JodaSazi(tok,"on="));
            if (ton == 0 && tok.rfind("ton=",0)==0) ton = harfadad(JodaSazi(tok,"ton="));
            if (ton == 0) { std::stringstream scan(line); std::string t2; while (scan >> t2) if (t2.rfind("ton=",0)==0) { ton = harfadad(JodaSazi(t2,"ton=")); break; } }
            circuit.addPulseCurrentSource(idown, iup, tperiod, trise, tfall, ton, name, ensure_node(p1_name), ensure_node(p2_name));
            apply_ui_rect(name);
        }
        else if (Typee == "IDELTA") {
            ss >> name >> tok; p1_name = JodaSazi(tok,"p1=");
            ss >> tok;         p2_name = JodaSazi(tok,"p2=");
            ss >> tok;         tperiod = harfadad(JodaSazi(tok,"tp="));
            circuit.addDeltaCurrentSource(tperiod, name, ensure_node(p1_name), ensure_node(p2_name));
            apply_ui_rect(name);
        }
            // ── DEPENDENT SOURCES
        else if (Typee == "E") { // VCVS
            ss >> name >> tok; p1_name = JodaSazi(tok,"p1=");
            ss >> tok;         p2_name = JodaSazi(tok,"p2=");
            ss >> tok;         cp1_name= JodaSazi(tok,"cp1=");
            ss >> tok;         cp2_name= JodaSazi(tok,"cp2=");
            ss >> tok;         gain    = harfadad(JodaSazi(tok,"g="));
            circuit.addVCVS(gain, name,
                            ensure_node(p1_name), ensure_node(p2_name),
                            ensure_node(cp1_name), ensure_node(cp2_name));
            apply_ui_rect(name);
        }
        else if (Typee == "G") { // VCCS
            ss >> name >> tok; p1_name = JodaSazi(tok,"p1=");
            ss >> tok;         p2_name = JodaSazi(tok,"p2=");
            ss >> tok;         cp1_name= JodaSazi(tok,"cp1=");
            ss >> tok;         cp2_name= JodaSazi(tok,"cp2=");
            ss >> tok;         gm      = harfadad(JodaSazi(tok,"gm="));
            circuit.addVCCS(gm, name,
                            ensure_node(p1_name), ensure_node(p2_name),
                            ensure_node(cp1_name), ensure_node(cp2_name));
            apply_ui_rect(name);
        }
        else if (Typee == "H") { // CCVS
            ss >> name >> tok; p1_name = JodaSazi(tok,"p1=");
            ss >> tok;         p2_name = JodaSazi(tok,"p2=");
            ss >> tok;
            VoltageSource* control_vs = nullptr;
            if (tok.rfind("cs=",0)==0) {
                cs_name = JodaSazi(tok,"cs=");
                ss >> tok; rm = harfadad(JodaSazi(tok,"rm="));
                control_vs = dynamic_cast<VoltageSource*>(circuit.findComponent(cs_name));
                if (!control_vs) {
                    circuit.addVoltageSource(0.0, cs_name, ensure_node("N001"), ensure_node("N002"));
                    control_vs = dynamic_cast<VoltageSource*>(circuit.findComponent(cs_name));
                }
            } else {
                cp1_name = JodaSazi(tok,"cp1=");
                ss >> tok; cp2_name = JodaSazi(tok,"cp2=");
                ss >> tok; rm      = harfadad(JodaSazi(tok,"rm="));
                std::string ctrlName = name + "_CTRL";
                control_vs = dynamic_cast<VoltageSource*>(circuit.findComponent(ctrlName));
                if (!control_vs) {
                    circuit.addVoltageSource(0.0, ctrlName, ensure_node(cp1_name), ensure_node(cp2_name));
                    control_vs = dynamic_cast<VoltageSource*>(circuit.findComponent(ctrlName));
                } else {
                    control_vs->Nude1 = ensure_node(cp1_name);
                    control_vs->Nude2 = ensure_node(cp2_name);
                }
                if (control_vs) control_vs->IsMajaz = true;
            }
            circuit.addCCVS(rm, name, ensure_node(p1_name), ensure_node(p2_name), control_vs);
            apply_ui_rect(name);
        }
        else if (Typee == "F") { // CCCS
            ss >> name >> tok; p1_name = JodaSazi(tok,"p1=");
            ss >> tok;         p2_name = JodaSazi(tok,"p2=");
            ss >> tok;
            VoltageSource* control_vs = nullptr;
            if (tok.rfind("cs=",0)==0) {
                cs_name = JodaSazi(tok,"cs=");
                ss >> tok; gain = harfadad(JodaSazi(tok,"b="));
                control_vs = dynamic_cast<VoltageSource*>(circuit.findComponent(cs_name));
                if (!control_vs) {
                    circuit.addVoltageSource(0.0, cs_name, ensure_node("N001"), ensure_node("N002"));
                    control_vs = dynamic_cast<VoltageSource*>(circuit.findComponent(cs_name));
                }
            } else {
                cp1_name = JodaSazi(tok,"cp1=");
                ss >> tok; cp2_name = JodaSazi(tok,"cp2=");
                ss >> tok; gain     = harfadad(JodaSazi(tok,"b="));
                std::string ctrlName = name + "_CTRL";
                control_vs = dynamic_cast<VoltageSource*>(circuit.findComponent(ctrlName));
                if (!control_vs) {
                    circuit.addVoltageSource(0.0, ctrlName, ensure_node(cp1_name), ensure_node(cp2_name));
                    control_vs = dynamic_cast<VoltageSource*>(circuit.findComponent(ctrlName));
                } else {
                    control_vs->Nude1 = ensure_node(cp1_name);
                    control_vs->Nude2 = ensure_node(cp2_name);
                }
                if (control_vs) control_vs->IsMajaz = true;
            }
            circuit.addCCCS(gain, name, ensure_node(p1_name), ensure_node(p2_name), control_vs);
            apply_ui_rect(name);
        }
        else if (Typee == "DIODE") {
            ss >> name >> tok; p1_name = JodaSazi(tok,"p1=");
            ss >> tok;         p2_name = JodaSazi(tok,"p2=");
            circuit.addDiode(name, ensure_node(p1_name), ensure_node(p2_name), false);
            apply_ui_rect(name);
        }
        else if (Typee == "ZENER") {
            ss >> name >> tok; p1_name = JodaSazi(tok,"p1=");
            ss >> tok;         p2_name = JodaSazi(tok,"p2=");
            circuit.addDiode(name, ensure_node(p1_name), ensure_node(p2_name), true);
            apply_ui_rect(name);
        }
        // unknown Typee: ignore line
    }

    fin.close();
}

inline void read_circuit_from_file(const std::string& filename, Circuit& circuit) {
    read_circuit_from_file(filename, circuit, /*clearFirst=*/false);
}

// ─────────────────────────────────────────────────────────────────────────────
// SAVE
// ─────────────────────────────────────────────────────────────────────────────
void save_circuit_to_file(const std::string& filename, const Circuit& circuit) {
    // If a user-chosen path exists (gProjectPath), always save there
    // and never create an additional auto-named "project#.txt".
    extern std::string gProjectPath;

    // Helper: is this an auto-generated "project123.txt" style name?
    auto is_auto_project_name = [](const std::string& path)->bool {
        size_t slash = path.find_last_of("\\/");
        std::string base = (slash == std::string::npos) ? path : path.substr(slash + 1);
        // case-insensitive check: "project<digits>.txt"
        std::string b; b.reserve(base.size());
        for (unsigned char ch : base) b.push_back((char)std::tolower(ch));
        if (b.rfind("project", 0) != 0) return false;               // doesn't start with "project"
        if (b.size() < 12) return false;                             // too short for "project1.txt"
        if (b.substr(b.size() - 4) != ".txt") return false;          // must end with .txt
        // ensure the middle part is digits
        for (size_t i = 7; i + 4 <= b.size(); ++i) {                 // digits between "project" and ".txt"
            if (i == b.size() - 4) break;                            // stop at ".txt"
            if (!std::isdigit((unsigned char)b[i])) return false;
        }
        return true;
    };

    // Decide final target path
    std::string target = filename;
    if (!gProjectPath.empty()) {
        // If someone passes an auto-name while we already have a user path,
        // redirect to the user path to avoid duplicate files.
        if (target != gProjectPath && is_auto_project_name(target)) {
            target = gProjectPath;
        }
    }

    std::ofstream fout(target, std::ios::trunc);
    if (!fout.is_open()) { std::cout << "Error: Could not open file for writing: " << target << std::endl; return; }
    fout << std::fixed << std::setprecision(10);

    auto node_index = [](const std::string& n)->int {
        if (!n.empty() && (n[0]=='N' || n[0]=='n')) {
            int idx = 0;
            for (size_t i=1; i<n.size(); ++i) {
                if (std::isdigit(static_cast<unsigned char>(n[i]))) idx = idx*10 + (n[i]-'0');
                else break;
            }
            return idx;
        }
        return INT_MAX;
    };

    auto write_component = [&](const Component* comp){
        int x=0, y=0, ww=100, hh=44; // avoid shadowing; use ww/hh
        (void)ui_get_element_rect(comp->name, x, y, ww, hh);
        if (ww <= 0) ww = 100;
        if (hh <= 0) hh = 44;

        if (auto r = dynamic_cast<const Resistor*>(comp)) {
            fout << "R " << r->name << " p1=" << r->Nude1->name << " p2=" << r->Nude2->name
                 << " v=" << r->Value
                 << " x="<<x<<" y="<<y<<" w="<<ww<<" h="<<hh << "\n";
        }
        else if (auto vs = dynamic_cast<const VoltageSource*>(comp)) {
            if (auto svs = dynamic_cast<const SineVoltageSource*>(comp)) {
                fout << "VSIN " << svs->name << " p1=" << svs->Nude1->name << " p2=" << svs->Nude2->name
                     << " o=" << svs->Offset << " a=" << svs->Damane << " f=" << svs->Frequency
                     << " ph=" << (svs->Phase * 180.0 / M_PI)
                     << " x="<<x<<" y="<<y<<" w="<<ww<<" h="<<hh << "\n";
            } else if (auto pvs = dynamic_cast<const PulseVoltageSource*>(comp)) {
                fout << "VPULSE " << pvs->name << " p1=" << pvs->Nude1->name << " p2=" << pvs->Nude2->name
                     << " vd=" << pvs->VDown << " vu=" << pvs->VUp << " tp=" << pvs->TPeriod
                     << " tr=" << pvs->TRise << " tf=" << pvs->TFall << " ton=" << pvs->TOn
                     << " x="<<x<<" y="<<y<<" w="<<ww<<" h="<<hh << "\n";
            } else if (auto dvs = dynamic_cast<const DeltaVoltageSource*>(comp)) {
                fout << "VDELTA " << dvs->name << " p1=" << dvs->Nude1->name << " p2=" << dvs->Nude2->name
                     << " tp=" << dvs->TPeriod
                     << " x="<<x<<" y="<<y<<" w="<<ww<<" h="<<hh << "\n";
            } else {
                fout << "V " << vs->name << " p1=" << vs->Nude1->name << " p2=" << vs->Nude2->name
                     << " v=" << vs->Value
                     << " x="<<x<<" y="<<y<<" w="<<ww<<" h="<<hh << "\n";
            }
        }
        else if (auto cs = dynamic_cast<const CurrentSource*>(comp)) {
            if (auto scs = dynamic_cast<const SineCurrentSource*>(comp)) {
                fout << "ISIN " << scs->name << " p1=" << scs->Nude1->name << " p2=" << scs->Nude2->name
                     << " o=" << scs->Offset << " a=" << scs->Damane << " f=" << scs->Frequency
                     << " ph=" << (scs->Phase * 180.0 / M_PI)
                     << " x="<<x<<" y="<<y<<" w="<<ww<<" h="<<hh << "\n";
            } else if (auto pcs = dynamic_cast<const PulseCurrentSource*>(comp)) {
                fout << "IPULSE " << pcs->name << " p1=" << pcs->Nude1->name << " p2=" << pcs->Nude2->name
                     << " id=" << pcs->IDown << " iu=" << pcs->IUp << " tp=" << pcs->TPeriod
                     << " tr=" << pcs->TRise << " tf=" << pcs->TFall << " ton=" << pcs->TOn
                     << " x="<<x<<" y="<<y<<" w="<<ww<<" h="<<hh << "\n";
            } else if (auto dcs = dynamic_cast<const DeltaCurrentSource*>(comp)) {
                fout << "IDELTA " << dcs->name << " p1=" << dcs->Nude1->name << " p2=" << dcs->Nude2->name
                     << " tp=" << dcs->TPeriod
                     << " x="<<x<<" y="<<y<<" w="<<ww<<" h="<<hh << "\n";
            } else {
                fout << "I " << cs->name << " p1=" << cs->Nude1->name << " p2=" << cs->Nude2->name
                     << " v=" << cs->Value
                     << " x="<<x<<" y="<<y<<" w="<<ww<<" h="<<hh << "\n";
            }
        }
        else if (auto c = dynamic_cast<const Capacitor*>(comp)) {
            fout << "C " << c->name << " p1=" << c->Nude1->name << " p2=" << c->Nude2->name
                 << " c=" << c->Value << " iv=" << c->initialVoltage
                 << " x="<<x<<" y="<<y<<" w="<<ww<<" h="<<hh << "\n";
        }
        else if (auto l = dynamic_cast<const Inductor*>(comp)) {
            fout << "L " << l->name << " p1=" << l->Nude1->name << " p2=" << l->Nude2->name
                 << " l=" << l->Value << " ic=" << l->initialCurrent
                 << " x="<<x<<" y="<<y<<" w="<<ww<<" h="<<hh << "\n";
        }
        else if (auto vcvs = dynamic_cast<const VCVS*>(comp)) {
            fout << "E " << vcvs->name
                 << " p1=" << vcvs->Nude1->name << " p2=" << vcvs->Nude2->name
                 << " cp1=" << vcvs->ControlNude1->name << " cp2=" << vcvs->ControlNude2->name
                 << " g=" << vcvs->Gain
                 << " x="<<x<<" y="<<y<<" w="<<ww<<" h="<<hh << "\n";
        }
        else if (auto vccs = dynamic_cast<const VCCS*>(comp)) {
            fout << "G " << vccs->name
                 << " p1=" << vccs->Nude1->name << " p2=" << vccs->Nude2->name
                 << " cp1=" << vccs->ControlNude1->name << " cp2=" << vccs->ControlNude2->name
                 << " gm=" << vccs->GM
                 << " x="<<x<<" y="<<y<<" w="<<ww<<" h="<<hh << "\n";
        }
        else if (auto ccvs = dynamic_cast<const CCVS*>(comp)) {
            const auto* vs = ccvs->ControlVoltageSource;
            const bool writeCS = (vs && !vs->name.empty() && !vs->IsMajaz);
            if (writeCS) {
                fout << "H " << ccvs->name
                     << " p1=" << ccvs->Nude1->name << " p2=" << ccvs->Nude2->name
                     << " cs=" << vs->name
                     << " rm=" << ccvs->RM
                     << " x="<<x<<" y="<<y<<" w="<<ww<<" h="<<hh << "\n";
            } else {
                const std::string cp1 = (vs && vs->Nude1) ? vs->Nude1->name : "N001";
                const std::string cp2 = (vs && vs->Nude2) ? vs->Nude2->name : "N002";
                fout << "H " << ccvs->name
                     << " p1=" << ccvs->Nude1->name << " p2=" << ccvs->Nude2->name
                     << " cp1=" << cp1 << " cp2=" << cp2
                     << " rm=" << ccvs->RM
                     << " x="<<x<<" y="<<y<<" w="<<ww<<" h="<<hh << "\n";
            }
        }
        else if (auto cccs = dynamic_cast<const CCCS*>(comp)) {
            const auto* vs = cccs->ControlVoltageSource;
            const bool writeCS = (vs && !vs->name.empty() && !vs->IsMajaz);
            if (writeCS) {
                fout << "F " << cccs->name
                     << " p1=" << cccs->Nude1->name << " p2=" << cccs->Nude2->name
                     << " cs=" << vs->name
                     << " b=" << cccs->Gain
                     << " x="<<x<<" y="<<y<<" w="<<ww<<" h="<<hh << "\n";
            } else {
                const std::string cp1 = (vs && vs->Nude1) ? vs->Nude1->name : "N001";
                const std::string cp2 = (vs && vs->Nude2) ? vs->Nude2->name : "N002";
                fout << "F " << cccs->name
                     << " p1=" << cccs->Nude1->name << " p2=" << cccs->Nude2->name
                     << " cp1=" << cp1 << " cp2=" << cp2
                     << " b=" << cccs->Gain
                     << " x="<<x<<" y="<<y<<" w="<<ww<<" h="<<hh << "\n";
            }
        }
        else if (auto d = dynamic_cast<const Diode*>(comp)) {
            if (!d->IsZn)
                fout << "DIODE " << d->name << " p1=" << d->Nude1->name << " p2=" << d->Nude2->name;
            else
                fout << "ZENER " << d->name << " p1=" << d->Nude1->name << " p2=" << d->Nude2->name;
            fout << " x="<<x<<" y="<<y<<" w="<<ww<<" h="<<hh << "\n";
        }
    };

    // Collect nodes that appear in visible components
    std::unordered_set<const Node*> used;
    auto add2 = [&](const auto* obj){
        if (!obj) return;
        if (obj->Nude1) used.insert(obj->Nude1);
        if (obj->Nude2) used.insert(obj->Nude2);
    };

    for (const auto* comp : circuit.components) {
        if (!comp || comp->IsMajaz) continue;
        if (auto r  = dynamic_cast<const Resistor*>(comp))         { add2(r); }
        else if (auto c  = dynamic_cast<const Capacitor*>(comp))   { add2(c); }
        else if (auto l  = dynamic_cast<const Inductor*>(comp))    { add2(l); }
        else if (auto d  = dynamic_cast<const Diode*>(comp))       { add2(d); }
        else if (auto vs = dynamic_cast<const VoltageSource*>(comp)){ add2(vs); }
        else if (auto cs = dynamic_cast<const CurrentSource*>(comp)){ add2(cs); }
        else if (auto e  = dynamic_cast<const VCVS*>(comp)) {
            add2(e);
            if (e->ControlNude1) used.insert(e->ControlNude1);
            if (e->ControlNude2) used.insert(e->ControlNude2);
        } else if (auto g = dynamic_cast<const VCCS*>(comp)) {
            add2(g);
            if (g->ControlNude1) used.insert(g->ControlNude1);
            if (g->ControlNude2) used.insert(g->ControlNude2);
        } else if (auto h = dynamic_cast<const CCVS*>(comp)) {
            add2(h);
            if (h->ControlVoltageSource) {
                if (h->ControlVoltageSource->Nude1) used.insert(h->ControlVoltageSource->Nude1);
                if (h->ControlVoltageSource->Nude2) used.insert(h->ControlVoltageSource->Nude2);
            }
        } else if (auto f = dynamic_cast<const CCCS*>(comp)) {
            add2(f);
            if (f->ControlVoltageSource) {
                if (f->ControlVoltageSource->Nude1) used.insert(f->ControlVoltageSource->Nude1);
                if (f->ControlVoltageSource->Nude2) used.insert(f->ControlVoltageSource->Nude2);
            }
        }
    }

    // Unique-by-name & sorted
    std::unordered_map<std::string, const Node*> uniqueNodes;
    for (const Node* n : used) if (n && !n->name.empty()) uniqueNodes.emplace(n->name, n);

    std::vector<std::pair<std::string,const Node*>> nodesOut(uniqueNodes.begin(), uniqueNodes.end());
    std::sort(nodesOut.begin(), nodesOut.end(),
              [&](const auto& A, const auto& B){
                  int ia = node_index(A.first), ib = node_index(B.first);
                  if (ia != ib) return ia < ib;
                  return A.first < B.first;
              });

    // Write nodes
    for (const auto& kv : nodesOut) {
        const Node* node = kv.second;
        fout << "N " << node->name
             << " g=" << (node->IsG ? "1" : "0")
             << " m=" << (node->IsMajaz ? "1" : "0") << "\n";
    }

    // Write visible components only
    std::unordered_set<std::string> seenComp;
    for (const auto* comp : circuit.components) {
        if (!comp || comp->IsMajaz) continue;
        if (!seenComp.insert(comp->name).second) continue;
        write_component(comp);
    }

    fout.close();
}





void CircuitMenu(Circuit& cir) {
    cout << endl << (cir.currentfilee.empty() ? "NEW" : cir.currentfilee)  << "circuit opened" << endl;
    cout << "Ready for circuit commands. Type 'return' to return to the menu and 'end' to exit." << endl;
    while (true) {
        string command = getCommand(cir);

        if (command == "end") {
            exit(0);
        }
        if (command == "return") {
            return;
        }
        try {
            Manage manage(command);
            manage.add(cir);
        } catch (const runtime_error& e) {
            cout << e.what();
        }
        if (!cir.currentfilee.empty()) {
            save_circuit_to_file(cir.currentfilee, cir);
        }
    }
}
/* void showSchematicsMenu(Circuit& cir) {
    cout << "\n--- Available schematics (latest) ---" << endl;

    if (CircuitFiles.empty()) {
        cout << "No schematic files have been recently opened/saved." << endl;
    }
    else {
        for (size_t i = 0; i < CircuitFiles.size(); ++i) {
            cout << (i + 1) << "- " << CircuitFiles[i] << endl;
        }
    }

    cout << "\nEnter the desired schematic number to open (or 'return' to return to the main menu):" << endl;
    string ii;
    getline(cin, ii);

    if (regex_match(ii, regex(R"(^\s*end\s*$)"))) {
        exit(0);
    }
    if (regex_match(ii, regex(R"(^\s*return\s*$)"))) {
        return;
    }

    try {
        int Adad = stoi(ii);
        if (Adad > 0 && Adad <= CircuitFiles.size()) {
            string NumberSelect = CircuitFiles[Adad - 1];

            ifstream testFile(NumberSelect);
            if (!testFile.is_open()) {
                cout << "Error: File " << NumberSelect << " not found or accessible." << endl;
                CircuitFiles.erase(CircuitFiles.begin() + Adad - 1);
                saveCircuitFiles();
                return;
            }

            cout << "\n--- File Content: " << NumberSelect << " ---" << endl;
            string line;
            int lineNumber = 1;
            while (getline(testFile, line)) {
                cout << lineNumber++ << ": " << line << endl;
            }
            testFile.close();

            cout << "\nDo you want to open this file? (yes/no)" << endl;
            string confirmation;
            getline(cin, confirmation);

            if (regex_match(confirmation, regex(R"(^\s*end\s*$)"))) {
                exit(0);
            }
            if (regex_match(confirmation, regex(R"(^\s*no\s*$)"))) {
                showSchematicsMenu(cir);
                return;
            }
            if (!regex_match(confirmation, regex(R"(^\s*yes\s*$)"))) {
                cout << "-Error : Inappropriate input" << endl;
                showSchematicsMenu(cir);
                return;
            }
            cir.clearCircuit();
            read_circuit_from_file(NumberSelect, cir);

            if (!cir.nodes.empty() || !cir.components.empty()) {
                cir.currentfilee = NumberSelect;
                CircuitMenu(cir);
            } else {
                cout << "Error: Schematic file " << NumberSelect << " is empty or invalid. A new schematic will be created in it." << endl;
                cir.currentfilee = NumberSelect;
                CircuitMenu(cir);
            }

        }
        else {
            cout << "-Error : Inappropriate input" << endl;
        }
    }
    catch (const invalid_argument& e) {
        cout << "-Error : Inappropriate input" << endl;
    }
    catch (const out_of_range& e) {
        cout << "Error: The entered number is out of range." << endl;
    }
}
void openOrCreateFile(Circuit& cir) {
    cout << "\n--- Open/Create New File ---" << endl;
    cout << R"(Please enter the full file address (including name and format, example: C:\Users\User\circuit.txt))" << endl;
    cout << "Type 'return' to return to the menu and 'end' to exit." << endl;

    string filee;
    getline(cin, filee);

    if (regex_match(filee, regex(R"(^\s*end\s*$)"))) {
        exit(0);
    }
    if (regex_match(filee, regex(R"(^\s*return\s*$)"))) {
        return;
    }

    if (filee.empty()) {
        cout << "Error: File address cannot be empty." << endl;
        return;
    }
    cir.clearCircuit();

    ifstream testFile(filee);
    if (testFile.is_open()) {
        testFile.close();
        cout << "File " << filee << " found. Loading..." << endl;
        read_circuit_from_file(filee, cir);

        if (!cir.nodes.empty() || !cir.components.empty()) {
            cir.currentfilee = filee;
            AddCircuitFile(filee);
            CircuitMenu(cir);
        }
        else {
            cout << "The file " << filee << " is empty or invalid. A new schematic will be created in it." << endl;
            cir.currentfilee = filee;
            AddCircuitFile(filee);
            CircuitMenu(cir);
        }
    }
    else {
        cout << "File " << filee << " not found. Creating new file..." << endl;
        ofstream newFile(filee);
        if (newFile.is_open()) {
            newFile.close();
            cout << "New file " << filee << " created successfully." << endl;
            cir.currentfilee = filee;
            AddCircuitFile(filee);
            CircuitMenu(cir);
        }
        else {
            cout << "Error: Unable to create file " << filee << ". Invalid path/filename." << endl;
            return;
        }
    }
} */
// ---------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------



// helper: map component class -> palette label you use in UI
static std::string UITypeForComponent(const Component* comp) {
    if (dynamic_cast<const Resistor*>(comp))        return "Resistor";
    if (dynamic_cast<const Capacitor*>(comp))       return "Capacitor";
    if (dynamic_cast<const Inductor*>(comp))        return "Inductor";
    if (auto d = dynamic_cast<const Diode*>(comp))  return d->IsZn ? "Zener" : "Diode";

    if (auto sv = dynamic_cast<const SineVoltageSource*>(comp))   return "SINE_V";
    if (auto pv = dynamic_cast<const PulseVoltageSource*>(comp))  return "PULSE_V";
    if (auto dv = dynamic_cast<const DeltaVoltageSource*>(comp))  return "DELTA_V";
    if (dynamic_cast<const VoltageSource*>(comp))                 return "Vsrc";

    if (auto si = dynamic_cast<const SineCurrentSource*>(comp))   return "SINE_I";
    if (auto pi = dynamic_cast<const PulseCurrentSource*>(comp))  return "PULSE_I";
    if (auto di = dynamic_cast<const DeltaCurrentSource*>(comp))  return "DELTA_I";
    if (dynamic_cast<const CurrentSource*>(comp))                 return "Isrc";

    if (dynamic_cast<const VCVS*>(comp)) return "VCVS";
    if (dynamic_cast<const VCCS*>(comp)) return "VCCS";
    if (dynamic_cast<const CCVS*>(comp)) return "CCVS";
    if (dynamic_cast<const CCCS*>(comp)) return "CCCS";
    return "Resistor"; // fallback
}

static inline void append_xywh_if_present(std::ostream& out, const std::string& name) {
    auto it = gUiPos.find(name);
    if (it != gUiPos.end()) {
        out << " x=" << it->second.x
            << " y=" << it->second.y
            << " w=" << it->second.w
            << " h=" << it->second.h;
    }
}
// --- Text helper (SDL_ttf) ---
static TTF_Font* loadUIFont(int px = 20) {
    static bool ttfInit = false;
    if (!ttfInit) { if (TTF_Init()!=0) std::cerr << "TTF_Init: " << TTF_GetError() << "\n"; ttfInit = true; }
    TTF_Font* f = TTF_OpenFont("C:\\Windows\\Fonts\\consola.ttf", px);
    if (!f) f = TTF_OpenFont("C:\\Windows\\Fonts\\arial.ttf", px);
    if (!f) std::cerr << "TTF_OpenFont failed\n";
    return f;
}
static void drawText(SDL_Renderer* r, TTF_Font* f, const std::string& s, int x, int y,
                     SDL_Color col = {255,255,255,255}) {
    if (!f) return;
    SDL_Surface* surf = TTF_RenderUTF8_Blended(f, s.c_str(), col);
    if (!surf) return;
    SDL_Texture* tex = SDL_CreateTextureFromSurface(r, surf);
    SDL_FreeSurface(surf);
    if (!tex) return;
    SDL_Rect dst{ x, y, 0, 0 };
    SDL_QueryTexture(tex, nullptr, nullptr, &dst.w, &dst.h);
    SDL_RenderCopy(r, tex, nullptr, &dst);
    SDL_DestroyTexture(tex);
}


// make sure D:\OOP\shemas\ exists (no <filesystem> / no windows.h)
static inline void EnsureShemasFolder() {
    system("cmd /C if not exist \"D:\\OOP\\shemas\" mkdir \"D:\\OOP\\shemas\"");
}

// open if exists, otherwise create with a tiny header
static inline void OpenOrCreateProjectFile(const std::string& path) {
    EnsureShemasFolder();
    std::ifstream in(path);
    if (in.good()) { in.close(); return; }      // already exists
    std::ofstream out(path);
    if (out.is_open()) {
        out << "# OOP schematic file\n";
        out.close();
    }
}

// Pick the first available D:\OOP\shemas\projectN.txt (N = 1..9999)
static inline std::string NextProjectPath() {
    EnsureShemasFolder();                  // you already added this earlier
    for (int n = 1; n <= 9999; ++n) {
        std::string path = "D:\\OOP\\shemas\\project" + std::to_string(n) + ".txt";
        std::ifstream in(path);
        if (!in.good()) return path;      // not found -> use this name
    }
    return "D:\\OOP\\shemas\\project9999.txt"; // extreme fallback
}

// List D:\OOP\shemas\project*.txt without <filesystem>/WinAPI
static inline std::vector<std::string> ListProjectFiles() {
    EnsureShemasFolder();
    // dump the bare file names into a temp file, then read it
    system("cmd /C dir /b /a:-d \"D:\\OOP\\shemas\\project*.txt\" > \"D:\\OOP\\shemas\\_list.tmp\"");
    std::ifstream in("D:\\OOP\\shemas\\_list.tmp");
    std::vector<std::string> out;
    std::string line;
    while (std::getline(in, line)) {
        if (!line.empty())
            out.push_back(std::string("D:\\OOP\\shemas\\") + line);
    }
    in.close();
    system("cmd /C del /q \"D:\\OOP\\shemas\\_list.tmp\" >nul 2>nul");
    return out;
}

// Call this from your SDL layer whenever you want to add an element
// line example: "add R1 n1 n2 1k"
// ==== ADD: tiny bridge to phase-one parser ====
inline bool AddElementFromText(Circuit& cir, const std::string& line, std::string* err = nullptr) {
    try { Manage(line).add(cir); return true; }
    catch (const std::exception& e) { if (err) *err = e.what(); return false; }
}

extern std::string gProjectPath;

// Append one command line to the active project file
static inline void AppendProjectLine(const std::string& line) {
    if (gProjectPath.empty()) return;
    std::ofstream out(gProjectPath.c_str(), std::ios::app);
    if (out) out << line << "\n";
}

// Wrap parser + persist to file when it succeeds
static inline bool AddAndPersist(Circuit& cir, const std::string& cmd) {
    bool ok = AddElementFromText(cir, cmd);
    if (ok) AppendProjectLine(cmd);
    return ok;
}




static std::string NextNode() {
    std::ostringstream os; os << "N" << std::setw(3) << std::setfill('0') << gNodeCounter++;
    return os.str();
}
struct Two { std::string a, b; };
static Two twoNodes() { Two t; t.a = NextNode(); t.b = NextNode(); return t; }

static Node* EnsureNode(Circuit& cir, const std::string& name) {
    Node* n = cir.findNode(name);
    if (n) return n;
    return cir.addNode(name);
}

static VoltageSource* AnyVoltageSourcePtr(Circuit& cir) {
    for (auto* c : cir.components) {
        if (auto* vs = dynamic_cast<VoltageSource*>(c)) return vs;
    }
    return nullptr;
}

static int gVID_ref = 100000;
static VoltageSource* EnsureControlVSourcePtr(Circuit& cir) {
    if (auto* vs = AnyVoltageSourcePtr(cir)) return vs;
    // Create a tiny DC ref VS via API
    Two t = twoNodes();
    Node* p = EnsureNode(cir, t.a);
    Node* n = EnsureNode(cir, t.b);
    std::string name = "Vref" + std::to_string(gVID_ref++);
    cir.addVoltageSource(1.0, name, p, n);
    return dynamic_cast<VoltageSource*>(cir.findComponent(name));
}
// --------------------------------------------------------------------


// -------------- REPLACE your AddElementForType with this --------------
bool AddElementForType(Circuit& cir, const std::string& type, std::string* outName = nullptr)
{
    std::string cmd;

    // SIMPLE ONES: keep using text parser (works fine)
    if (type == "Resistor")   { Two t=twoNodes(); cmd="add R"+std::to_string(gRID++)+" "+t.a+" "+t.b+" 1k"; }
    else if (type == "Capacitor"){ Two t=twoNodes(); cmd="add C"+std::to_string(gCID++)+" "+t.a+" "+t.b+" 10u"; }
    else if (type == "Inductor"){  Two t=twoNodes(); cmd="add L"+std::to_string(gLID++)+" "+t.a+" "+t.b+" 1m"; }
    else if (type == "Diode")     { Two t=twoNodes(); cmd="add D"+std::to_string(gDID++)+" "+t.a+" "+t.b; }
    else if (type == "Zener")     { Two t=twoNodes(); cmd="add DZ"+std::to_string(gZID++)+" "+t.a+" "+t.b; }

        // INDEPENDENT VOLTAGE SOURCES
    else if (type == "Vsrc") {
        // DC Voltage source via Phase-1 API (NOT the text parser)
        Two t = twoNodes();
        Node* p = EnsureNode(cir, t.a);
        Node* n = EnsureNode(cir, t.b);
        std::string name = "V" + std::to_string(gVID++);
        double Vdc = 5.0;
        cir.addVoltageSource(Vdc, name, p, n);
        if (outName) *outName = name;
        std::cout << "Added DC Voltage Source: " << name << "\n";
        return true;  // IMPORTANT: return here so we never fall through to the parser
    }
    else if (type == "SINE_V")    { Two t=twoNodes(); cmd="add V"+std::to_string(gVID++)+" "+t.a+" "+t.b+" SIN(0 5 1k)"; }
    else if (type == "DELTA_V")   { Two t=twoNodes(); cmd="add V"+std::to_string(gVID++)+" "+t.a+" "+t.b+" DELTA(10m)"; }
    else if (type == "PULSE_V") {
        // USE API: vd, vu, tp, tr, tf, ton
        Two t = twoNodes(); Node* p=EnsureNode(cir,t.a); Node* n=EnsureNode(cir,t.b);
        std::string name = "V" + std::to_string(gVID++);
        // defaults (tweak as you like):
        double vd=0, vu=5, tp=0.001, tr=0.001, tf=0.001, ton=0.010;
        cir.addPulseVoltageSource(vd, vu, tp, tr, tf, ton, name, p, n);
        std::cout << "Added Pulse Voltage Source: " << name << "\n";
        if (outName) *outName = name;
        return true;
    }

        // INDEPENDENT CURRENT SOURCES
    else if (type == "Isrc") {
        // DC Current source via Phase-1 API (NOT the text parser)
        Two t = twoNodes();
        Node* p = EnsureNode(cir, t.a);
        Node* n = EnsureNode(cir, t.b);
        std::string name = "I" + std::to_string(gIID++);
        double Idc = 1e-3;
        cir.addCurrentSource(Idc, name, p, n);
        if (outName) *outName = name;
        std::cout << "Added DC Current Source: " << name << "\n";
        return true;  // IMPORTANT
    }
    else if (type == "SINE_I")    { Two t=twoNodes(); cmd="add I"+std::to_string(gIID++)+" "+t.a+" "+t.b+" SIN(0 1m 1k)"; }
    else if (type == "DELTA_I")   { Two t=twoNodes(); cmd="add I"+std::to_string(gIID++)+" "+t.a+" "+t.b+" DELTA(10m)"; }
    else if (type == "PULSE_I") {
        // USE API: id, iu, tp, tr, tf, ton
        Two t = twoNodes(); Node* p=EnsureNode(cir,t.a); Node* n=EnsureNode(cir,t.b);
        std::string name = "I" + std::to_string(gIID++);
        double id=0, iu=0.001, tp=0.001, tr=0.001, tf=0.001, ton=0.010;
        cir.addPulseCurrentSource(id, iu, tp, tr, tf, ton, name, p, n);
        std::cout << "Added Pulse Current Source: " << name << "\n";
        if (outName) *outName = name;
        return true;
    }
        // CONTROLLED SOURCES
    else if (type == "VCVS") { // E: p n cp1 cp2 gain
        Two t1=twoNodes(), t2=twoNodes();
        cmd = "add E"+std::to_string(gEID++)+" "+t1.a+" "+t1.b+" "+t2.a+" "+t2.b+" 10";
    }
    else if (type == "VCCS") { // G: p n cp1 cp2 gm
        Two t1=twoNodes(), t2=twoNodes();
        cmd = "add G"+std::to_string(gGID++)+" "+t1.a+" "+t1.b+" "+t2.a+" "+t2.b+" 1m";
    }
    else if (type == "CCVS") { // H
        Two t = twoNodes(); Node* p = EnsureNode(cir, t.a); Node* n = EnsureNode(cir, t.b);
        std::string name = "H" + std::to_string(gHID++);
        VoltageSource* ctrl = EnsureControlVSourcePtr(cir);
        if (!ctrl) { std::cerr << "Cannot make CCVS: no control VS\n"; return false; }
        double rm = 1.0;
        cir.addCCVS(rm, name, p, n, ctrl);
        if (outName) *outName = name;
        std::cout << "Added CCVS: " << name << " (cs=" << ctrl->name << ")\n";
        return true;
    }

    else if (type == "CCCS") { // F
        Two t = twoNodes(); Node* p = EnsureNode(cir, t.a); Node* n = EnsureNode(cir, t.b);
        std::string name = "F" + std::to_string(gFID++);
        VoltageSource* ctrl = EnsureControlVSourcePtr(cir);
        if (!ctrl) { std::cerr << "Cannot make CCCS: no control VS\n"; return false; }
        double gain = 1.0;
        cir.addCCCS(gain, name, p, n, ctrl);
        if (outName) *outName = name;
        std::cout << "Added CCCS: " << name << " (cs=" << ctrl->name << ")\n";
        return true;
    }
    else {
        std::cout << "Unknown type: " << type << "\n";
        return false;
    }

    // For the types that still use the text parser:
    bool ok = AddElementFromText(cir, cmd);
    if (!ok) {
        std::cerr << "Failed to parse: " << cmd << "\n";
        return false;
    }
    if (outName) {
        std::istringstream iss(cmd);
        std::string addTok; iss >> addTok; // "add"
        std::string name;   iss >> name;   // R1 / V2 / ...
        *outName = name;
    }
    return true;
}



static std::string fmt10(double v) {
    std::ostringstream os; os.setf(std::ios::fixed); os<<std::setprecision(10)<<v;
    return os.str();
}

// ---- One dialog for N numeric fields (supports SI like 1k, 10m using harfadad) ----
// Edit N numeric values in one small modal window.
// labels.size() is how many fields you want.
// inoutValues.size() should be >= labels.size(); extra values are ignored.
// If allowUnits is true, inputs like "1k", "10m" are parsed via harfadad().
bool EditNumbersDialog(const std::string& windowTitle,
                       const std::vector<std::string>& labels,
                       std::vector<double>& inoutValues,
                       bool allowUnits)
{
    const int W = 520, H = 120 + (int)labels.size()*70;
    SDL_Window* win = SDL_CreateWindow(windowTitle.c_str(),
                                       SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                       W, H, SDL_WINDOW_SHOWN);
    if (!win) return false;

    SDL_Renderer* r = SDL_CreateRenderer(win, -1,
                                         SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!r) { SDL_DestroyWindow(win); return false; }

    TTF_Font* fontTitle = loadUIFont(20);
    TTF_Font* font      = loadUIFont(18);

    // Prepare fields
    const int N = (int)labels.size();
    if ((int)inoutValues.size() < N) inoutValues.resize(N, 0.0);

    struct Field {
        SDL_Rect box;
        std::string label;
        std::string text;
    };
    std::vector<Field> fields; fields.reserve(N);

    int y0 = 60;
    for (int i = 0; i < N; ++i) {
        Field f;
        f.box.x = 20; f.box.y = y0; f.box.w = W - 40; f.box.h = 40;
        f.label = labels[i];
        {
            std::ostringstream ss; ss << std::setprecision(12) << inoutValues[i];
            f.text = ss.str();
        }
        fields.push_back(f);
        y0 += 60;
    }

    SDL_Rect btnOK;      btnOK.x = W - 200; btnOK.y = H - 56; btnOK.w = 80; btnOK.h = 36;
    SDL_Rect btnCancel;  btnCancel.x = W - 100; btnCancel.y = H - 56; btnCancel.w = 80; btnCancel.h = 36;

    // Helpers
    struct { bool operator()(const SDL_Rect& a, int x, int y) const {
            return x>=a.x && x<=a.x+a.w && y>=a.y && y<=a.y+a.h;
        }} inside;

    // Parse helper (no std::optional; returns bool + out param)
    auto parseTextToDouble = [&](const std::string& s, double& out) -> bool {
        if (allowUnits) {
            try { out = harfadad(s); return true; } catch(...) {}
        }
        try { out = std::stod(s); return true; } catch (...) {}
        return false;
    };

    // UI state
    int active = 0;
    bool cursor = true; Uint32 lastBlink = SDL_GetTicks();
    bool running = true, accepted = false;

    SDL_StartTextInput();
    while (running) {
        SDL_Event e; int mx=0,my=0; SDL_GetMouseState(&mx,&my);
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) { running = false; break; }

            if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_ESCAPE) { running = false; break; }

                if (e.key.keysym.sym == SDLK_TAB) {
                    if ((e.key.keysym.mod & KMOD_SHIFT) != 0) {
                        active = (active - 1 + N) % N;
                    } else {
                        active = (active + 1) % N;
                    }
                }

                if (e.key.keysym.sym == SDLK_BACKSPACE) {
                    if (!fields[active].text.empty()) fields[active].text.pop_back();
                }

                if (e.key.keysym.sym == SDLK_RETURN || e.key.keysym.sym == SDLK_KP_ENTER) {
                    // Try to parse all fields
                    bool okAll = true;
                    for (int i=0;i<N;++i) {
                        double val;
                        if (!parseTextToDouble(fields[i].text, val)) { okAll = false; active = i; break; }
                    }
                    if (okAll) {
                        for (int i=0;i<N;++i) {
                            double val; parseTextToDouble(fields[i].text, val);
                            inoutValues[i] = val;
                        }
                        accepted = true;
                        running = false;
                        break;
                    }
                }
            }

            if (e.type == SDL_TEXTINPUT) {
                const char* s = e.text.text;
                while (*s) {
                    char c = *s++;
                    if (std::isprint((unsigned char)c)) {
                        // Accept digits and typical numeric chars; allow unit suffixes if allowed
                        bool ok = std::isdigit((unsigned char)c) || c=='+'||c=='-'||c=='.'||c=='e'||c=='E';
                        if (allowUnits) {
                            if (c=='k'||c=='K'||c=='m'||c=='u'||c=='U'||c=='n'||c=='N'||c=='p'||c=='P')
                                ok = true;
                        }
                        if (ok) fields[active].text.push_back(c);
                    }
                }
            }

            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                if (inside(btnCancel, mx, my)) { running=false; break; }
                if (inside(btnOK, mx, my)) {
                    bool okAll = true;
                    for (int i=0;i<N;++i) {
                        double val;
                        if (!parseTextToDouble(fields[i].text, val)) { okAll = false; active = i; break; }
                    }
                    if (okAll) {
                        for (int i=0;i<N;++i) {
                            double val; parseTextToDouble(fields[i].text, val);
                            inoutValues[i] = val;
                        }
                        accepted = true;
                        running = false;
                        break;
                    }
                }
                for (int i=0;i<N;++i) if (inside(fields[i].box, mx, my)) { active=i; break; }
            }
        }

        // caret blink
        if (SDL_GetTicks() - lastBlink > 500) { cursor = !cursor; lastBlink = SDL_GetTicks(); }

        // Draw
        SDL_SetRenderDrawColor(r, 24,28,38,255); SDL_RenderClear(r);
        drawText(r, fontTitle, windowTitle.c_str(), 20, 18);

        for (int i=0;i<N;++i) {
            // label
            drawText(r, font, fields[i].label.c_str(), fields[i].box.x, fields[i].box.y - 22);
            // box + text
            Uint8 br = (i==active ? 120 : 90), bg = (i==active ? 160 : 120), bb = (i==active ? 220 : 180);
            roundedRectangleRGBA(r, fields[i].box.x, fields[i].box.y,
                                 fields[i].box.x+fields[i].box.w, fields[i].box.y+fields[i].box.h,
                                 8, br,bg,bb,255);
            std::string shown = fields[i].text;
            if (i==active && cursor) shown += "|";
            drawText(r, font, shown.c_str(), fields[i].box.x+10, fields[i].box.y+10);
        }

        auto drawBtn = [&](SDL_Rect b, const char* cap){
            bool hot = inside(b,mx,my);
            roundedBoxRGBA(r, b.x,b.y,b.x+b.w,b.y+b.h, 8, hot?60:45, hot?160:120, hot?230:190, hot?255:220);
            roundedRectangleRGBA(r, b.x,b.y,b.x+b.w,b.y+b.h, 8, 40,70,120,255);
            drawText(r, font, cap, b.x+20, b.y+8);
        };
        drawBtn(btnOK, "OK");
        drawBtn(btnCancel, "Cancel");

        SDL_RenderPresent(r);
    }

    SDL_StopTextInput();
    if (font)      TTF_CloseFont(font);
    if (fontTitle) TTF_CloseFont(fontTitle);
    SDL_DestroyRenderer(r);
    SDL_DestroyWindow(win);
    return accepted;
}

static inline bool EditNumbersDialog(const std::string& windowTitle,
                                     const std::vector<std::string>& labels,
                                     std::vector<double>& inoutValues)
{
    return EditNumbersDialog(windowTitle, labels, inoutValues, /*allowUnits=*/true);
}
// ---- Single dispatcher for ALL components (R/C/L, V/I DC/SIN/PULSE/DELTA, E/G/H/F) ----
bool EditElementParameters(Component* c)
{
    if (!c) return false;

    // DC / Passive
    if (auto r = dynamic_cast<Resistor*>(c)) {
        std::vector<std::string> labels = {"Resistance (Ω)"};
        std::vector<double> vals = { r->Value };
        if (EditNumbersDialog(r->name, labels, vals)) { r->Value = vals[0]; return true; }
        return false;
    }
    if (auto cap = dynamic_cast<Capacitor*>(c)) {
        std::vector<std::string> labels = {"Capacitance (F)"};
        std::vector<double> vals = { cap->Value };
        if (EditNumbersDialog(cap->name, labels, vals)) { cap->Value = vals[0]; return true; }
        return false;
    }
    if (auto ind = dynamic_cast<Inductor*>(c)) {
        std::vector<std::string> labels = {"Inductance (H)"};
        std::vector<double> vals = { ind->Value };
        if (EditNumbersDialog(ind->name, labels, vals)) { ind->Value = vals[0]; return true; }
        return false;
    }

    // Voltage sources
    if (auto svs = dynamic_cast<SineVoltageSource*>(c)) {
        std::vector<std::string> labels = {"Offset (V)", "Amplitude (V)", "Frequency (Hz)"};
        std::vector<double> vals = { svs->Offset, svs->Damane, svs->Frequency };
        if (EditNumbersDialog(svs->name, labels, vals)) {
            svs->Offset = vals[0]; svs->Damane = vals[1]; svs->Frequency = vals[2]; return true;
        }
        return false;
    }
    if (auto pvs = dynamic_cast<PulseVoltageSource*>(c)) {
        std::vector<std::string> labels = {
                "Vdown (V)", "Vup (V)", "Tperiod (s)", "Trise (s)", "Tfall (s)", "Ton (s)"
        };
        std::vector<double> vals = { pvs->VDown, pvs->VUp, pvs->TPeriod, pvs->TRise, pvs->TFall, pvs->TOn };
        if (EditNumbersDialog(pvs->name, labels, vals)) {
            pvs->VDown=vals[0]; pvs->VUp=vals[1]; pvs->TPeriod=vals[2];
            pvs->TRise=vals[3]; pvs->TFall=vals[4]; pvs->TOn=vals[5]; return true;
        }
        return false;
    }
    if (auto dvs = dynamic_cast<DeltaVoltageSource*>(c)) {
        std::vector<std::string> labels = { "Tperiod (s)" };
        std::vector<double> vals = { dvs->TPeriod };
        if (EditNumbersDialog(dvs->name, labels, vals)) { dvs->TPeriod = vals[0]; return true; }
        return false;
    }
    if (auto vdc = dynamic_cast<VoltageSource*>(c)) {
        // Plain DC voltage (must come after the derived types)
        std::vector<std::string> labels = {"Voltage (V)"};
        std::vector<double> vals = { vdc->Value };
        if (EditNumbersDialog(vdc->name, labels, vals)) { vdc->Value = vals[0]; return true; }
        return false;
    }

    // Current sources
    if (auto scs = dynamic_cast<SineCurrentSource*>(c)) {
        std::vector<std::string> labels = {"Offset (A)", "Amplitude (A)", "Frequency (Hz)"};
        std::vector<double> vals = { scs->Offset, scs->Damane, scs->Frequency };
        if (EditNumbersDialog(scs->name, labels, vals)) {
            scs->Offset = vals[0]; scs->Damane = vals[1]; scs->Frequency = vals[2]; return true;
        }
        return false;
    }
    if (auto pcs = dynamic_cast<PulseCurrentSource*>(c)) {
        std::vector<std::string> labels = {
                "Idown (A)", "Iup (A)", "Tperiod (s)", "Trise (s)", "Tfall (s)", "Ton (s)"
        };
        std::vector<double> vals = { pcs->IDown, pcs->IUp, pcs->TPeriod, pcs->TRise, pcs->TFall, pcs->TOn };
        if (EditNumbersDialog(pcs->name, labels, vals)) {
            pcs->IDown=vals[0]; pcs->IUp=vals[1]; pcs->TPeriod=vals[2];
            pcs->TRise=vals[3]; pcs->TFall=vals[4]; pcs->TOn=vals[5]; return true;
        }
        return false;
    }
    if (auto dcs = dynamic_cast<DeltaCurrentSource*>(c)) {
        std::vector<std::string> labels = { "Tperiod (s)" };
        std::vector<double> vals = { dcs->TPeriod };
        if (EditNumbersDialog(dcs->name, labels, vals)) { dcs->TPeriod = vals[0]; return true; }
        return false;
    }
    if (auto idc = dynamic_cast<CurrentSource*>(c)) {
        // Plain DC current (must come after the derived types)
        std::vector<std::string> labels = {"Current (A)"};
        std::vector<double> vals = { idc->Value };
        if (EditNumbersDialog(idc->name, labels, vals)) { idc->Value = vals[0]; return true; }
        return false;
    }

    // Dependent sources (single "gain" style)
    if (auto e = dynamic_cast<VCVS*>(c)) {
        std::vector<std::string> labels = {"Gain (g)"};
        std::vector<double> vals = { e->Gain };
        if (EditNumbersDialog(e->name, labels, vals)) { e->Gain = vals[0]; return true; }
        return false;
    }
    if (auto g = dynamic_cast<VCCS*>(c)) {
        std::vector<std::string> labels = {"Transconductance (gm)"};
        std::vector<double> vals = { g->GM };
        if (EditNumbersDialog(g->name, labels, vals)) { g->GM = vals[0]; return true; }
        return false;
    }
    if (auto h = dynamic_cast<CCVS*>(c)) {
        std::vector<std::string> labels = {"Transresistance (rm)"};
        std::vector<double> vals = { h->RM };
        if (EditNumbersDialog(h->name, labels, vals)) { h->RM = vals[0]; return true; }
        return false;
    }
    if (auto f = dynamic_cast<CCCS*>(c)) {
        std::vector<std::string> labels = {"Current gain (b)"};
        std::vector<double> vals = { f->Gain };
        if (EditNumbersDialog(f->name, labels, vals)) { f->Gain = vals[0]; return true; }
        return false;
    }

    // No editable parameters (e.g., diode)
    return false;
}


//===================== Transient
// ===================== Transient Solver (Phase-1 shim) =====================
// Minimal dense MNA + Backward Euler for R, C, L, V/I sources, VCVS, VCCS.
// NOTE: CCVS/CCCS are NOT stamped here (will be added later).
// This is only for a console "smoke test" from the editor.

#include <unordered_set>
#include <unordered_map>
#include <vector>

// Collect only nodes that are actually referenced by at least one (visible) component.
// NOTE: For CCVS/CCCS, we also include the control-voltage-source pins.
static std::unordered_set<Node*> CollectUsedNodes(const Circuit& C)
{
    std::unordered_set<Node*> used;

    auto touch = [&](Node* n){ if (n) used.insert(n); };

    for (auto* comp : C.components) {
        if (!comp) continue;
        if (comp->IsMajaz) {
            // Skip hidden helpers (e.g., V*_CTRL). Their nodes will be pulled in
            // when we walk the owning CCVS/CCCS below.
            continue;
        }

        // 2-pin family
        if (auto r = dynamic_cast<Resistor*>(comp))         { touch(r->Nude1); touch(r->Nude2); continue; }
        if (auto c = dynamic_cast<Capacitor*>(comp))        { touch(c->Nude1); touch(c->Nude2); continue; }
        if (auto l = dynamic_cast<Inductor*>(comp))         { touch(l->Nude1); touch(l->Nude2); continue; }
        if (auto d = dynamic_cast<Diode*>(comp))            { touch(d->Nude1); touch(d->Nude2); continue; }
        if (auto v = dynamic_cast<VoltageSource*>(comp))    { touch(v->Nude1); touch(v->Nude2); continue; }
        if (auto i = dynamic_cast<CurrentSource*>(comp))    { touch(i->Nude1); touch(i->Nude2); continue; }

        // 4-pin family (explicit pins)
        if (auto e = dynamic_cast<VCVS*>(comp)) {
            touch(e->Nude1);        touch(e->Nude2);
            touch(e->ControlNude1); touch(e->ControlNude2);
            continue;
        }
        if (auto g = dynamic_cast<VCCS*>(comp)) {
            touch(g->Nude1);        touch(g->Nude2);
            touch(g->ControlNude1); touch(g->ControlNude2);
            continue;
        }
        if (auto h = dynamic_cast<CCVS*>(comp)) {
            touch(h->Nude1); touch(h->Nude2);
            if (h->ControlVoltageSource) {
                touch(h->ControlVoltageSource->Nude1);
                touch(h->ControlVoltageSource->Nude2);
            }
            continue;
        }
        if (auto f = dynamic_cast<CCCS*>(comp)) {
            touch(f->Nude1); touch(f->Nude2);
            if (f->ControlVoltageSource) {
                touch(f->ControlVoltageSource->Nude1);
                touch(f->ControlVoltageSource->Nude2);
            }
            continue;
        }
    }
    return used;
}

// Map node* to equation index; returns -1 for ground or unmapped nodes.
static inline int idx_or_neg1(Node* n, const std::unordered_map<Node*,int>& map)
{
    if (!n) return -1;
    if (n->IsG) return -1;                 // treat ground specially
    auto it = map.find(n);
    return (it==map.end()) ? -1 : it->second;
}

// Safe stamping helpers (examples for G matrix and RHS vector)
template <class Mat>
static inline void addG(Mat& G, int r, int c, double v) {
    if (r>=0 && c>=0) G(r,c) += v;
}
template <class Vec>
static inline void addI(Vec& b, int r, double v) {
    if (r>=0) b[r] += v;
}


struct TransientResult {
    std::vector<double> t;

    // node voltages: V[i] corresponds to nodeNames[i]
    std::vector<std::string> nodeNames;
    std::vector<std::vector<double>> V;
    std::unordered_map<std::string, size_t> nodeIndex;

    // element currents: I[j] corresponds to elemNamesI[j]
    std::vector<std::string> elemNamesI;
    std::vector<std::vector<double>> I;
    std::unordered_map<std::string, size_t> elemIndexI;
};

// ---------- node indexer (exclude grounds from unknown vector) ----------
struct NodeIndex {
    std::vector<Node*> allNodes;      // for UI/result listing (keeps all non-majaz nodes)
    std::vector<int>   allToVIdx;     // map allNodes[i] -> voltage unknown index or -1
    std::unordered_map<Node*, int> vIndex; // only for used, non-ground nodes
    int nV = 0;

    NodeIndex(const Circuit& c) {
        // 1) Find nodes actually referenced by at least one (non-majaz) component
        std::unordered_set<Node*> used;
        auto mark2 = [&](Node* a, Node* b){ if(a) used.insert(a); if(b) used.insert(b); };
        auto mark4 = [&](Node* a, Node* b, Node* c1, Node* c2){
            if(a) used.insert(a); if(b) used.insert(b);
            if(c1) used.insert(c1); if(c2) used.insert(c2);
        };

        for (auto* comp : c.components) {
            if (!comp || comp->IsMajaz) continue;
            if (auto* r  = dynamic_cast<Resistor*>(comp))        mark2(r->Nude1, r->Nude2);
            else if (auto* cp = dynamic_cast<Capacitor*>(comp))  mark2(cp->Nude1, cp->Nude2);
            else if (auto* l  = dynamic_cast<Inductor*>(comp))   mark2(l->Nude1, l->Nude2);
            else if (auto* d  = dynamic_cast<Diode*>(comp))      mark2(d->Nude1, d->Nude2);
            else if (auto* vs = dynamic_cast<VoltageSource*>(comp)) mark2(vs->Nude1, vs->Nude2);
            else if (auto* cs = dynamic_cast<CurrentSource*>(comp)) mark2(cs->Nude1, cs->Nude2);
            else if (auto* e  = dynamic_cast<VCVS*>(comp))       mark4(e->Nude1, e->Nude2, e->ControlNude1, e->ControlNude2);
            else if (auto* g  = dynamic_cast<VCCS*>(comp))       mark4(g->Nude1, g->Nude2, g->ControlNude1, g->ControlNude2);
            else if (auto* h  = dynamic_cast<CCVS*>(comp)) {
                mark2(h->Nude1, h->Nude2);
                if (h->ControlVoltageSource) mark2(h->ControlVoltageSource->Nude1, h->ControlVoltageSource->Nude2);
            } else if (auto* f = dynamic_cast<CCCS*>(comp)) {
                mark2(f->Nude1, f->Nude2);
                if (f->ControlVoltageSource) mark2(f->ControlVoltageSource->Nude1, f->ControlVoltageSource->Nude2);
            }
        }

        // 2) Keep all non-majaz nodes for UI list; only index the used, non-ground ones
        for (auto* n : c.nodes) if (n && !n->IsMajaz) allNodes.push_back(n);

        allToVIdx.assign(allNodes.size(), -1);
        vIndex.clear(); nV = 0;
        for (size_t i=0; i<allNodes.size(); ++i) {
            Node* n = allNodes[i];
            if (!n) continue;
            if (n->IsG) { allToVIdx[i] = -1; continue; }    // ground never gets an unknown
            if (used.find(n) == used.end()) {               // exclude floating/unused nodes (by real parts)
                allToVIdx[i] = -1;
                continue;
            }
            allToVIdx[i] = nV;
            vIndex[n] = nV;
            ++nV;
        }
    }

    inline int idx(Node* n) const {
        if (!n) return -1;
        auto it = vIndex.find(n);
        return (it == vIndex.end()) ? -1 : it->second;
    }
};

// ----------------------------------------------------------------------------
// Transient (Phase-1) with GMIN anchors + stepping on singular MNA
// -----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// Transient (Phase-1) with GMIN anchors + stepping on singular MNA
// -----------------------------------------------------------------------------
bool SolveTransient_Phase1(Circuit& c, double dt, double tstop, TransientResult& out)
{
    if (dt <= 0.0 || tstop < 0.0) return false;

    // 0) Start clean (aligns with Phase-1 style like SolveAC)
    c.resetState();

    // 0.1) Make sure time-dependent and companion elements are up-to-date
    //      *before* structure init, so the very first matrix is well-posed.
    for (Component* comp : c.components) {
        if (auto* cap = dynamic_cast<Capacitor*>(comp))       cap->updateValues(dt);
        else if (auto* ind = dynamic_cast<Inductor*>(comp))   ind->updateValues(dt);
        comp->updateTimeDependentValue(0.0);
    }
    for (Component* comp : c.components) {
        if (auto* delt = dynamic_cast<DeltaVoltageSource*>(comp))
            delt->Epsilon = (dt > 0.0 ? dt : 1e-18);
    }

    // 0.2) Identify a ground node (first IsG=true)
    Node* ground = nullptr;
    for (Node* n : c.nodes) { if (n && n->IsG) { ground = n; break; } }
    if (!ground) return false; // safety; UI should guarantee one

    // 0.3) Inject tiny GMIN anchors (Majaz resistors) from every non-ground node to ground.
    //      These guarantee no all-zero KCL rows even in pathological first-step cases.
    //      We'll remove them at the end.
    const double RGMIN = 1e12;             // 1e12 Ω => 1e-12 S
    std::vector<Resistor*> gmin_added;
    gmin_added.reserve(c.nodes.size());
    auto ensure_gmin = [&](Node* n) {
        if (!n || n->IsG) return;
        std::string gname = "__GMIN_" + n->name;
        if (c.findComponent(gname)) return;          // already present
        // Manually create the Majaz resistor and add it
        Resistor* r = new Resistor();
        r->name  = gname;
        r->Nude1 = n;
        r->Nude2 = ground;
        r->Value = RGMIN;
        r->IsMajaz = true;
        c.components.push_back(r);
        gmin_added.push_back(r);
    };
    for (Node* n : c.nodes) ensure_gmin(n);

    // ---- Topology view for results (stable across the whole run) ----
    NodeIndex NI(c);
    const int nV = NI.nV;

    out.t.clear();
    out.nodeNames.clear();
    out.nodeNames.reserve(NI.allNodes.size());
    for (auto* nd : NI.allNodes) out.nodeNames.push_back(nd->name);

    out.V.assign(out.nodeNames.size(), {});
    out.nodeIndex.clear();
    for (size_t i = 0; i < out.nodeNames.size(); ++i)
        out.nodeIndex[out.nodeNames[i]] = static_cast<int>(i);

    // Which element currents to export (skip Majaz helpers)
    std::vector<const Component*> elemsIList;
    out.elemNamesI.clear();
    out.elemIndexI.clear();
    auto addITrace = [&](const Component* comp) {
        out.elemIndexI[comp->name] = static_cast<int>(out.elemNamesI.size());
        out.elemNamesI.push_back(comp->name);
        elemsIList.push_back(comp);
    };
    for (auto* comp : c.components) {
        if (!comp || comp->IsMajaz) continue;
        if (dynamic_cast<Resistor*>(comp)      ||
            dynamic_cast<Capacitor*>(comp)     ||
            dynamic_cast<Inductor*>(comp)      ||
            dynamic_cast<VoltageSource*>(comp) ||
            dynamic_cast<CurrentSource*>(comp) ||
            dynamic_cast<VCVS*>(comp)          ||
            dynamic_cast<VCCS*>(comp)          ||
            dynamic_cast<Diode*>(comp))
        {
            addITrace(comp);
        }
    }
    out.I.assign(out.elemNamesI.size(), {});

    // ---- Phase-1 solver structure (unknowns and source lists) ----
    CircuitSolver solver;
    if (!solver.initializeCircuitStructure(c)) {
        std::cout << "[Transient] initializeCircuitStructure failed.\n";
        // Clean up GMIN before returning
        for (auto* r : gmin_added) c.deleteComponentPointer(r);
        return false;
    }

    // Diode list for SolveDiodes()
    c.diodesInCircuit.clear();
    for (Component* comp : c.components)
        if (auto* d = dynamic_cast<Diode*>(comp)) c.diodesInCircuit.push_back(d);

    // Previous node voltages (for BE capacitor current)
    std::vector<double> v_prev_nodes(nV, 0.0);

    // ---- Time marching ----
    const int steps = static_cast<int>(std::ceil((tstop <= 0.0 ? 0.0 : tstop) / dt));
    double t = 0.0;

    for (int k = 0; k <= steps; ++k, t = std::min(t + dt, tstop)) {

        // Update L/C companions and time-dependent sources for this step
        for (Component* comp : c.components) {
            if (auto* cap = dynamic_cast<Capacitor*>(comp))       cap->updateValues(dt);
            else if (auto* ind = dynamic_cast<Inductor*>(comp))   ind->updateValues(dt);
            comp->updateTimeDependentValue(t);
        }

        // Solve with diode state recursion (Phase-1)
        if (!c.SolveDiodes(solver, 0)) {
            std::cout << "[Transient] SolveDiodes failed at t=" << std::fixed << std::setprecision(8) << t << "\n";
            // Clean up GMIN before returning
            for (auto* r : gmin_added) c.deleteComponentPointer(r);
            return false;
        }

        // Record time + node voltages (NodeIndex order)
        out.t.push_back(t);
        for (size_t iAll = 0; iAll < NI.allNodes.size(); ++iAll) {
            Node* nd = NI.allNodes[iAll];
            out.V[iAll].push_back(nd ? nd->Voltage : 0.0);
        }

        // Snapshot for capacitor currents
        const std::vector<double> v_prev_copy = v_prev_nodes;
        auto Vn = [&](Node* n) -> double { return n ? n->Voltage : 0.0; };

        // Element currents (p1 -> p2)
        for (size_t ei = 0; ei < elemsIList.size(); ++ei) {
            const Component* comp = elemsIList[ei];
            double I = 0.0;

            if (auto* R = dynamic_cast<const Resistor*>(comp)) {
                if (R->Value > 0.0)
                    I = (Vn(R->Nude1) - Vn(R->Nude2)) / R->Value;
            }
            else if (auto* Cc = dynamic_cast<const Capacitor*>(comp)) {
                if (Cc->Value > 0.0) {
                    // Backward-Euler: I = C/dt * [ (v_now) - (v_prev) ]
                    int ia = NI.idx(Cc->Nude1), ib = NI.idx(Cc->Nude2);
                    double va_now  = Vn(Cc->Nude1), vb_now  = Vn(Cc->Nude2);
                    double va_prev = (ia >= 0 && ia < (int)v_prev_copy.size()) ? v_prev_copy[ia] : 0.0;
                    double vb_prev = (ib >= 0 && ib < (int)v_prev_copy.size()) ? v_prev_copy[ib] : 0.0;
                    I = (Cc->Value / dt) * ((va_now - vb_now) - (va_prev - vb_prev));
                }
            }
            else if (auto* L = dynamic_cast<const Inductor*>(comp)) {
                if (L->MVoltageSource) I = L->MVoltageSource->Iv;
                else if (L->MResistor && L->MResistor->Value > 0.0)
                    I = (Vn(L->Nude1) - Vn(L->Nude2)) / L->MResistor->Value;
            }
            else if (auto* Vs = dynamic_cast<const VoltageSource*>(comp)) {
                I = Vs->Iv; // independent VS current
            }
            else if (auto* E = dynamic_cast<const VCVS*>(comp)) {
                I = E->Iv;  // controlled VS current
            }
            else if (auto* G = dynamic_cast<const VCCS*>(comp)) {
                double vc = Vn(G->ControlNude1) - Vn(G->ControlNude2);
                I = G->Value * vc;
            }
            else if (auto* D = dynamic_cast<const Diode*>(comp)) {
                if (D->MVoltageProbe) I = D->MVoltageProbe->Iv;
            }

            out.I[ei].push_back(I);
        }

        // Update state for next step (Phase-1 behavior)
        for (Component* comp : c.components) {
            if (auto* cap = dynamic_cast<Capacitor*>(comp)) {
                cap->VoltageGabl = Vn(cap->Nude1) - Vn(cap->Nude2);
            } else if (auto* ind = dynamic_cast<Inductor*>(comp)) {
                if (ind->MVoltageSource) ind->CurrentGabl = ind->MVoltageSource->Iv;
            }
        }

        // Store node voltages for BE in next step
        for (size_t iAll = 0; iAll < NI.allNodes.size(); ++iAll) {
            int ivi = NI.allToVIdx[iAll];
            if (ivi >= 0) v_prev_nodes[ivi] = out.V[iAll].back();
        }
    }

    // Remove injected GMIN anchors (keep the circuit pristine)
    for (auto* r : gmin_added) c.deleteComponentPointer(r);

    return true;
}


struct ACSweepResult {
    std::vector<double> freq;                   // sweep frequencies (Hz)

    // Node voltages (phasors) in the same order across all f
    std::vector<std::string> nodeNames;
    std::unordered_map<std::string, size_t> nodeIndex;
    std::vector<std::vector<double>> Vmag;      // [node][k]  magnitude at freq[k]
    std::vector<std::vector<double>> Vphase;    // [node][k]  phase (rad) at freq[k]

    // Element currents (phasors) in the same order across all f
    std::vector<std::string> elemNamesI;
    std::unordered_map<std::string, size_t> elemIndexI;
    std::vector<std::vector<double>> Imag;      // [elem][k]
    std::vector<std::vector<double>> Iphase;    // [elem][k]
};

#include <complex>
#include <cmath>
#include <algorithm>

// Helper: build frequency vector
static inline std::vector<double>
BuildSweep(double f_start, double f_stop, int n_points, bool logspace) {
    std::vector<double> f;
    f.reserve(std::max(0, n_points));
    if (n_points < 2 || f_start <= 0.0 || f_stop < f_start) return f;

    if (logspace) {
        const double loga = std::log10(f_start);
        const double logb = std::log10(f_stop);
        for (int i = 0; i < n_points; ++i) {
            double t = (n_points==1 ? 0.0 : double(i)/double(n_points-1));
            f.push_back(std::pow(10.0, loga + t*(logb-loga)));
        }
    } else {
        const double step = (f_stop - f_start) / double(n_points - 1);
        for (int i = 0; i < n_points; ++i) f.push_back(f_start + i*step);
    }
    return f;
}

// Helper: single-tone phasor from the last window using trapezoidal rule
static inline std::complex<double>
FundamentalPhasor(const std::vector<double>& t,
                  const std::vector<double>& x,
                  double f)
{
    const double T = 1.0 / f;
    if (t.size() < 2) return {0.0, 0.0};

    // pick samples within the last period [t_end - T, t_end]
    const double t_end = t.back();
    const double t_start = t_end - T;

    // find first index >= t_start
    size_t i0 = 0;
    while (i0 + 1 < t.size() && t[i0] < t_start) ++i0;

    // integrate x(t) * e^{-j ω t} over that window
    const double w = 2.0 * M_PI * f;
    std::complex<double> S(0.0, 0.0);
    double covered = 0.0;

    for (size_t i = i0 + 1; i < t.size(); ++i) {
        double t1 = t[i-1], t2 = t[i];
        if (t2 < t_start) continue;

        // clip first partial segment to t_start
        double a = std::max(t1, t_start);
        double b = t2;
        if (b <= a) continue;

        std::complex<double> e1 = std::exp(std::complex<double>(0, -w * a));
        std::complex<double> e2 = std::exp(std::complex<double>(0, -w * b));

        // linear interp of x at 'a' if needed
        double xa = x[i-1];
        if (t2 > t1 && a > t1)
            xa = x[i-1] + (x[i] - x[i-1]) * (a - t1) / (t2 - t1);

        // trapezoidal on [a,b]
        double xb = x[i];
        std::complex<double> contrib = 0.5 * (xa * e1 + xb * e2) * (b - a);
        S += contrib;
        covered += (b - a);
    }

    if (covered <= 0.0) return {0.0, 0.0};

    // Fourier convention to get amplitude/phase of the fundamental:
    //   X1 = (2/T) ∫ x(t) e^{-j ω t} dt  over one full period
    return (2.0 / T) * S;
}

// ---------------- AC Sweep (Transient-powered) ----------------
bool SolveAC_Sweep_Phase1(Circuit& c,
                          double f_start, double f_stop, int n_points, bool logspace,
                          int pts_per_period, int settle_cycles, int measure_cycles,
                          ACSweepResult& out)
{
    // basic guards
    if (f_start <= 0.0 || f_stop < f_start || n_points < 2) return false;
    if (pts_per_period < 32) pts_per_period = 32;
    if (settle_cycles < 0)  settle_cycles = 0;
    if (measure_cycles < 1) measure_cycles = 1;

    // build frequency vector
    std::vector<double> freqs = BuildSweep(f_start, f_stop, n_points, logspace);
    if (freqs.empty()) return false;

    // collect all sine sources to retarget their frequency each step
    struct VSRef { SineVoltageSource* p; double f_orig; };
    struct CSRef { SineCurrentSource* p; double f_orig; };
    std::vector<VSRef> vsList;
    std::vector<CSRef> csList;

    for (Component* comp : c.components) {
        if (auto* sv = dynamic_cast<SineVoltageSource*>(comp)) {
            vsList.push_back({sv, sv->Frequency});
        } else if (auto* si = dynamic_cast<SineCurrentSource*>(comp)) {
            csList.push_back({si, si->Frequency});
        }
    }

    // We'll re-use the node/element layout from the *first* run to keep rows stable.
    bool layoutInitialized = false;

    out.freq.clear();
    out.nodeNames.clear();
    out.nodeIndex.clear();
    out.elemNamesI.clear();
    out.elemIndexI.clear();
    out.Vmag.clear();
    out.Vphase.clear();
    out.Imag.clear();
    out.Iphase.clear();

    // sweep loop
    for (int k = 0; k < (int)freqs.size(); ++k) {
        double f = freqs[k];
        if (f <= 0.0) continue;

        // retarget all sine sources to this frequency
        for (auto& v : vsList) v.p->Frequency = f;
        for (auto& i : csList) i.p->Frequency = f;

        // choose dt and tstop from the tone
        const double T = 1.0 / f;
        const double dt  = T / std::max(1, pts_per_period);
        const double tstop = (settle_cycles + measure_cycles) * T;

        // run the transient for this tone
        TransientResult tr;
        if (!SolveTransient_Phase1(c, dt, tstop, tr)) {
            // restore frequencies before returning
            for (auto& v : vsList) v.p->Frequency = v.f_orig;
            for (auto& i : csList) i.p->Frequency = i.f_orig;
            return false;
        }

        // initialize output layout from the first run
        if (!layoutInitialized) {
            out.nodeNames = tr.nodeNames;
            for (size_t i = 0; i < out.nodeNames.size(); ++i)
                out.nodeIndex[out.nodeNames[i]] = i;

            out.elemNamesI = tr.elemNamesI;
            for (size_t j = 0; j < out.elemNamesI.size(); ++j)
                out.elemIndexI[out.elemNamesI[j]] = j;

            out.Vmag.assign(out.nodeNames.size(), {});
            out.Vphase.assign(out.nodeNames.size(), {});
            out.Imag.assign(out.elemNamesI.size(), {});
            out.Iphase.assign(out.elemNamesI.size(), {});
            layoutInitialized = true;
        }

        // compute phasors from the last cycle for all nodes and element currents
        std::vector<double>& tt = const_cast<std::vector<double>&>(tr.t); // local alias

        // nodes
        for (size_t ni = 0; ni < tr.V.size(); ++ni) {
            std::complex<double> X = FundamentalPhasor(tt, tr.V[ni], f);
            out.Vmag[ni].push_back(std::abs(X));
            out.Vphase[ni].push_back(std::arg(X));
        }
        // element currents
        for (size_t ei = 0; ei < tr.I.size(); ++ei) {
            std::complex<double> X = FundamentalPhasor(tt, tr.I[ei], f);
            out.Imag[ei].push_back(std::abs(X));
            out.Iphase[ei].push_back(std::arg(X));
        }

        out.freq.push_back(f);
    }

    // restore original sine frequencies
    for (auto& v : vsList) v.p->Frequency = v.f_orig;
    for (auto& i : csList) i.p->Frequency = i.f_orig;

    return true;
}





// ===================== Live ResultsWindow (multi-series, dual-axis) =====================


extern TTF_Font* loadUIFont(int px);

// ---------- Series ----------
struct PlotSeries {
    enum class Axis { Left, Right };

    std::string label;           // legend label
    std::string key;             // unique lookup key (lowercased; no spaces)
    std::vector<double> t;       // time base
    std::vector<double> y;       // samples
    SDL_Color   color{30,130,220,255};
    std::string unit = "";       // "V","A","W",...
    Axis        axis  = Axis::Left;

    PlotSeries() = default;
    PlotSeries(std::string lbl,
               const std::vector<double>& tt,
               const std::vector<double>& yy,
               SDL_Color col = SDL_Color{30,130,220,255})
            : label(std::move(lbl)), t(tt), y(yy), color(col) {}
};

// ---------- ResultsWindow ----------
class ResultsWindow {
public:
    explicit ResultsWindow(const std::string& title, int w=980, int h=600)
            : title_(title), W_(w), H_(h)
    {
        win_ = SDL_CreateWindow(title_.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                W_, H_, SDL_WINDOW_SHOWN);
        if (!win_) return;
        ren_ = SDL_CreateRenderer(win_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (!ren_) { SDL_DestroyWindow(win_); win_ = nullptr; return; }

        fTitle_ = loadUIFont(18);
        fAxis_  = loadUIFont(14);
        fUI_    = loadUIFont(14);

        windowID_ = SDL_GetWindowID(win_);
        open_ = true;

        exprBuf_[0] = 0;
        exprActive_ = false;

        refreshGlobalTimeRange(true);
    }
    ~ResultsWindow() { destroy(); }

    bool isOpen()   const { return open_; }
    bool isClosed() const { return !open_; }
    void bringToFront()   { if (win_) SDL_RaiseWindow(win_); }

    // Add a new plotted series
    void addSeries(PlotSeries s) {
        if (!isOpen()) return;

        // unique key & default color
        s.key = uniqueKey(normalizeKey(s.label));
        if (s.color.a==0 && s.color.r==0 && s.color.g==0 && s.color.b==0)
            s.color = presetColors_[ series_.size() % presetColors_.size() ];

        // auto-axis: first left; if unit differs from left, prefer right
        if (series_.empty()) {
            s.axis = PlotSeries::Axis::Left;
        } else {
            std::string baseUnit = firstUnitOnAxis(PlotSeries::Axis::Left);
            if (!baseUnit.empty() && !s.unit.empty() && s.unit != baseUnit)
                s.axis = PlotSeries::Axis::Right;
        }

        series_.push_back(std::move(s));
        refreshGlobalTimeRange(false);
        if (std::fabs(viewTmax_ - viewTmin_) < 1e-12) {
            viewTmin_ = gTmin_; viewTmax_ = gTmax_;
        }
    }

    // ---------- Events ----------
    void handleEvent(const SDL_Event& e) {
        if (!isOpen()) return;

        if (e.type == SDL_WINDOWEVENT && e.window.windowID == windowID_) {
            if (e.window.event == SDL_WINDOWEVENT_CLOSE) { destroy(); return; }
        }

        if (e.type == SDL_KEYDOWN && e.key.windowID == windowID_) {
            if (exprActive_) {
                if (e.key.keysym.sym == SDLK_ESCAPE) { exprActive_ = false; SDL_StopTextInput(); return; }
                if (e.key.keysym.sym == SDLK_RETURN) { commitExpression(); return; }
            } else {
                if (e.key.keysym.sym == SDLK_ESCAPE) { if (showPalette_) showPalette_=false; else { destroy(); return; } }
                double span = (viewTmax_ - viewTmin_), step = 0.2 * span;
                if (e.key.keysym.sym == SDLK_LEFT)  { viewTmin_ -= step; viewTmax_ -= step; clampViewToGlobal(); }
                if (e.key.keysym.sym == SDLK_RIGHT) { viewTmin_ += step; viewTmax_ += step; clampViewToGlobal(); }
                if (e.key.keysym.sym == SDLK_EQUALS || e.key.keysym.sym == SDLK_PLUS)
                    zoomAround((viewTmin_+viewTmax_)*0.5, 0.8);
                if (e.key.keysym.sym == SDLK_MINUS)
                    zoomAround((viewTmin_+viewTmax_)*0.5, 1.18);
                if (e.key.keysym.sym == SDLK_HOME)  { viewTmin_ = gTmin_; viewTmax_ = gTmax_; }
            }
        }
        if (e.type == SDL_TEXTINPUT && e.text.windowID == windowID_) {
            if (exprActive_) {
                size_t L = strlen(exprBuf_), add = strlen(e.text.text);
                if (L + add < sizeof(exprBuf_) - 1) strcat(exprBuf_, e.text.text);
            }
        }
        if (e.type == SDL_KEYDOWN && e.key.windowID == windowID_) {
            if (exprActive_ && e.key.keysym.sym == SDLK_BACKSPACE) {
                size_t L = strlen(exprBuf_); if (L>0) exprBuf_[L-1]=0;
            }
        }

        if (e.type == SDL_MOUSEWHEEL && e.wheel.windowID == windowID_) {
            int mx, my; SDL_GetMouseState(&mx, &my);
            if (inPlot(mx, my)) {
                int y = e.wheel.y;
                if (e.wheel.direction == SDL_MOUSEWHEEL_FLIPPED) y = -y;
                zoomAround(xToT(mx), (y>0)? 0.85 : 1.18);
            }
        }

        if (e.type == SDL_MOUSEMOTION && e.motion.windowID == windowID_) {
            if (draggingPan_) {
                SDL_Rect p = plotRect();
                double dtPerPx = (viewTmax_ - viewTmin_) / std::max(1, p.w);
                double dtShift = -(e.motion.x - dragStartX_) * dtPerPx;
                viewTmin_ = dragStartTmin_ + dtShift;
                viewTmax_ = dragStartTmax_ + dtShift;
                clampViewToGlobal();
            }
        }

        if (e.type == SDL_MOUSEBUTTONDOWN && e.button.windowID == windowID_) {
            int mx=e.button.x, my=e.button.y;

            // Close
            SDL_Rect btnClose{ W_ - 84, 10, 74, 28 };
            if (e.button.button==SDL_BUTTON_LEFT && inRect(btnClose, mx, my)) { destroy(); return; }

            // Palette
            if (showPalette_ && e.button.button==SDL_BUTTON_LEFT) {
                if (inRect(paletteRc_, mx, my)) {
                    int pidx = paletteIndexAt(paletteRc_, mx, my);
                    if (pidx >= 0 && paletteTargetIndex_>=0 &&
                        paletteTargetIndex_<(int)series_.size())
                        series_[paletteTargetIndex_].color = presetColors_[pidx];
                    showPalette_ = false; return;
                } else showPalette_ = false;
            }

            // Legend (LMB palette / RMB axis toggle)
            std::vector<SDL_Rect> legendRects = buildLegendRectsTopRight(plotRect());
            int hit = legendHit(legendRects, mx, my);
            if (hit >= 0) {
                activeSeries_ = hit;
                if (e.button.button == SDL_BUTTON_LEFT) {
                    paletteTargetIndex_ = hit;
                    paletteRc_ = layoutPaletteRectTopRight(plotRect(), legendRects);
                    showPalette_ = true;
                } else if (e.button.button == SDL_BUTTON_RIGHT) {
                    auto& ax = series_[hit].axis;
                    ax = (ax==PlotSeries::Axis::Left ? PlotSeries::Axis::Right
                                                     : PlotSeries::Axis::Left);
                }
                return;
            }

            // Expression widgets
            SDL_Rect exprInput, exprAdd; layoutExprBar(exprInput, exprAdd);
            if (e.button.button == SDL_BUTTON_LEFT) {
                if (inRect(exprInput, mx, my)) { exprActive_ = true; SDL_StartTextInput(); return; }
                if (inRect(exprAdd,   mx, my)) { commitExpression(); return; }
            }
            if (exprActive_) { exprActive_ = false; SDL_StopTextInput(); }

            // Pan
            if ((e.button.button == SDL_BUTTON_LEFT || e.button.button == SDL_BUTTON_MIDDLE) &&
                inPlot(mx,my)) {
                draggingPan_ = true; dragStartX_ = mx;
                dragStartTmin_ = viewTmin_; dragStartTmax_ = viewTmax_;
                return;
            }

            // Reset view
            if (e.button.button == SDL_BUTTON_RIGHT && inPlot(mx,my)) {
                viewTmin_ = gTmin_; viewTmax_ = gTmax_; return;
            }
        }
        if (e.type == SDL_MOUSEBUTTONUP && e.button.windowID == windowID_) {
            if (e.button.button==SDL_BUTTON_LEFT || e.button.button==SDL_BUTTON_MIDDLE)
                draggingPan_ = false;
        }
    }

    // ---------- Rendering ----------
    void renderFrame() {
        if (!isOpen()) return;

        SDL_Rect plot = plotRect();

        // Visible Y ranges
        auto yL = visibleYRangeAxis(PlotSeries::Axis::Left,  viewTmin_, viewTmax_);
        auto yR = visibleYRangeAxis(PlotSeries::Axis::Right, viewTmin_, viewTmax_);
        if (!hasAxis(PlotSeries::Axis::Left))  yL = yR;
        if (!hasAxis(PlotSeries::Axis::Right)) yR = yL;

        double yminL=yL.first, ymaxL=yL.second;
        double yminR=yR.first, ymaxR=yR.second;
        if (!(ymaxL>yminL)) ymaxL = yminL + 1.0;
        if (!(ymaxR>yminR)) ymaxR = yminR + 1.0;

        // --- NEW: if both axes carry the same (or effectively the only) unit, unify ranges
        std::string uLeft  = firstUnitOnAxis(PlotSeries::Axis::Left);
        std::string uRight = firstUnitOnAxis(PlotSeries::Axis::Right);
        bool unifyLeftRight = false;
        std::string unitToUnify;
        if (!uLeft.empty() && uLeft == uRight)           { unifyLeftRight = true; unitToUnify = uLeft; }
        else if (!uLeft.empty() && uRight.empty())       { unifyLeftRight = true; unitToUnify = uLeft; }
        else if (!uRight.empty() && uLeft.empty())       { unifyLeftRight = true; unitToUnify = uRight; }
        if (unifyLeftRight) {
            auto yBoth = visibleYRangeForUnit(unitToUnify, viewTmin_, viewTmax_);
            yminL = yminR = yBoth.first;
            ymaxL = ymaxR = yBoth.second;
        }

        // clear
        SDL_SetRenderDrawColor(ren_, 252,252,255,255);
        SDL_RenderClear(ren_);

        // header
        drawText(ren_, fTitle_, title_, 12, 8, SDL_Color{40,60,90,255});
        drawButton(ren_, fUI_, SDL_Rect{ W_ - 84, 10, 74, 28 }, "Close");

        // plot area
        SDL_SetRenderDrawColor(ren_, 225,230,240,255);
        SDL_RenderFillRect(ren_, &plot);
        SDL_SetRenderDrawColor(ren_, 160,175,200,255);
        SDL_RenderDrawRect(ren_, &plot);

        // main frame axes
        drawThickH(ren_, plot.x, plot.x+plot.w, plot.y+plot.h, SDL_Color{40,60,90,255});
        drawThickV(ren_, plot.y, plot.y+plot.h, plot.x,         SDL_Color{40,60,90,255});

        // grid/ticks
        drawXTicks(viewTmin_, viewTmax_);
        if (hasAxis(PlotSeries::Axis::Left))  drawYTicksLeft(yminL, ymaxL);
        if (hasAxis(PlotSeries::Axis::Right)) drawYTicksRight(yminR, ymaxR);

        // zero lines
        SDL_Color zeroCol{90,110,170,255};
        if (viewTmin_<=0.0 && 0.0<=viewTmax_) drawThickV(ren_, plot.y, plot.y+plot.h, Vx(0.0), zeroCol);
        if (hasAxis(PlotSeries::Axis::Left) && yminL<=0.0 && 0.0<=ymaxL)
            drawThickH(ren_, plot.x, plot.x+plot.w, VyLeft(0.0, yminL, ymaxL), zeroCol);

        // series
        for (const auto& s : series_) {
            bool left = (s.axis == PlotSeries::Axis::Left);
            drawSeries(s, viewTmin_, viewTmax_, left?yminL:yminR, left?ymaxL:ymaxR, left);
        }

        // legend top-right
        std::vector<SDL_Rect> legendRects = buildLegendRectsTopRight(plot);
        drawLegendTopRight(plot, legendRects);

        // cursor readout
        int mx,my; SDL_GetMouseState(&mx,&my);
        if (inPlot(mx,my)) {
            drawThickV(ren_, plot.y, plot.y+plot.h, mx, SDL_Color{120,130,160,160});
            double tSel = xToT(mx);
            std::vector<std::string> lines;
            lines.push_back(std::string("t = ") + fmt(tSel) + " s");
            for (size_t i=0;i<series_.size(); ++i) {
                const PlotSeries& s = series_[i];
                double v = sampleAt(s, tSel);
                std::string ax = (s.axis==PlotSeries::Axis::Left? "[L]" : "[R]");
                std::string uni = s.unit.empty()? "" : (" " + s.unit);
                lines.push_back("#" + std::to_string(int(i+1)) + " " + ax + " " + s.label + " : " + fmt(v) + uni);
            }
            drawPanel(lines, plot.x + 8, plot.y + 8);
        }

        // palette (if open)
        if (showPalette_) drawPalette(paletteRc_);

        // axis unit labels
        if (hasAxis(PlotSeries::Axis::Left))
            drawText(ren_, fAxis_, unitForAxis(PlotSeries::Axis::Left), 8, plot.y+4, SDL_Color{40,60,90,255});
        if (hasAxis(PlotSeries::Axis::Right)) {
            std::string ur = unitForAxis(PlotSeries::Axis::Right);
            int tw=0,th=0; textSize(fAxis_, ur, tw, th);
            drawText(ren_, fAxis_, ur, plot.x + plot.w - tw - 4, plot.y+4, SDL_Color{40,60,90,255});
        }

        // bottom expression bar
        drawBottomBar();

        SDL_RenderPresent(ren_);
    }

private:
    // ---------- lifecycle ----------
    void destroy() {
        if (!open_) return;
        open_ = false;
        if (fUI_)    { TTF_CloseFont(fUI_);    fUI_ = nullptr; }
        if (fAxis_)  { TTF_CloseFont(fAxis_);  fAxis_ = nullptr; }
        if (fTitle_) { TTF_CloseFont(fTitle_); fTitle_ = nullptr; }
        if (ren_)    { SDL_DestroyRenderer(ren_); ren_ = nullptr; }
        if (win_)    { SDL_DestroyWindow(win_);   win_ = nullptr; }
    }

    // ---------- time ranges ----------
    void refreshGlobalTimeRange(bool initView) {
        bool first=true; double tmin=0, tmax=0;
        for (const auto& s : series_) {
            if (s.t.empty()) continue;
            if (first) { tmin = s.t.front(); tmax = s.t.back(); first=false; }
            else { tmin = std::min(tmin, s.t.front()); tmax = std::max(tmax, s.t.back()); }
        }
        if (first) { tmin=0; tmax=1; }
        gTmin_ = tmin; gTmax_ = tmax;
        if (!(gTmax_ > gTmin_)) gTmax_ = gTmin_ + 1.0;
        if (initView) { viewTmin_ = gTmin_; viewTmax_ = gTmax_; }
    }
    void clampViewToGlobal() {
        double span = viewTmax_ - viewTmin_;
        double gspan = std::max(1e-12, gTmax_ - gTmin_);
        if (span < gspan * 1e-6) {
            double mid = 0.5*(viewTmin_+viewTmax_);
            span = gspan * 1e-6;
            viewTmin_ = mid - 0.5*span;
            viewTmax_ = mid + 0.5*span;
        }
        if (viewTmin_ < gTmin_) { double d = gTmin_ - viewTmin_; viewTmin_ += d; viewTmax_ += d; }
        if (viewTmax_ > gTmax_) { double d = viewTmax_ - gTmax_; viewTmin_ -= d; viewTmax_ -= d; }
        if (viewTmin_ < gTmin_) viewTmin_ = gTmin_;
        if (viewTmax_ > gTmax_) viewTmax_ = gTmax_;
    }
    void zoomAround(double anchorT, double factor) {
        double L = anchorT - viewTmin_;
        double R = viewTmax_ - anchorT;
        L *= factor; R *= factor;
        viewTmin_ = anchorT - L;
        viewTmax_ = anchorT + R;
        clampViewToGlobal();
    }

    // ---------- geometry ----------
    SDL_Rect plotRect() const { return SDL_Rect{ Lm_, Tm_, W_ - Lm_ - Rm_, H_ - Tm_ - Bm_ }; }
    static bool inRect(const SDL_Rect& r, int x, int y) { return x>=r.x && x<=r.x+r.w && y>=r.y && y<=r.y+r.h; }
    bool inPlot(int x, int y) const { return inRect(plotRect(), x, y); }
    int  plotW() const { return W_ - Lm_ - Rm_; }
    int  plotH() const { return H_ - Tm_ - Bm_; }
    int  Vx(double t) const {
        return Lm_ + int((t - viewTmin_) * plotW() / std::max(1e-30, (viewTmax_ - viewTmin_)));
    }
    int  VyLeft(double y, double ymin, double ymax) const {
        SDL_Rect p = plotRect();
        return p.y + p.h - int((y - ymin) * p.h / std::max(1e-30, (ymax - ymin)));
    }
    int  VyRight(double y, double ymin, double ymax) const { return VyLeft(y, ymin, ymax); }
    double xToT(int x) const {
        return viewTmin_ + (double)(x - Lm_) * (viewTmax_ - viewTmin_) / std::max(1, plotW());
    }

    // ---------- sampling ----------
    static size_t nearestIndex(const std::vector<double>& t, double tv) {
        if (t.empty()) return 0;
        auto it = std::lower_bound(t.begin(), t.end(), tv);
        if (it == t.end()) return t.size() - 1;
        size_t idx = size_t(it - t.begin());
        if (idx > 0 && (tv - t[idx-1]) < (t[idx] - tv)) --idx;
        return idx;
    }
    static double sampleAt(const PlotSeries& s, double tv) {
        if (s.t.empty() || s.y.empty()) return 0.0;
        size_t i = nearestIndex(s.t, tv);
        return (i < s.y.size()) ? s.y[i] : 0.0;
    }
    static double sampleAtEndInView(const PlotSeries& s, double tmin, double tmax) {
        if (s.t.empty() || s.y.empty()) return 0.0;
        auto it = std::upper_bound(s.t.begin(), s.t.end(), tmax);
        if (it == s.t.begin()) return s.y.front();
        size_t idx = size_t((it - s.t.begin()) - 1);
        return (idx < s.y.size()) ? s.y[idx] : s.y.back();
    }

    // ---------- y-range ----------
    std::pair<double,double> visibleYRangeAxis(PlotSeries::Axis ax, double tmin, double tmax) const {
        bool first=true; double ymin=0, ymax=0;
        for (const auto& s : series_) {
            if (s.axis != ax) continue;
            if (s.t.empty() || s.y.empty()) continue;
            size_t i0 = nearestIndex(s.t, tmin);
            size_t i1 = nearestIndex(s.t, tmax);
            if (i0 > i1) std::swap(i0,i1);
            i1 = std::min(i1+1, s.y.size()-1);
            for (size_t i=i0; i<=i1; ++i) {
                if (first) { ymin=ymax=s.y[i]; first=false; }
                else { ymin = std::min(ymin, s.y[i]); ymax = std::max(ymax, s.y[i]); }
            }
        }
        if (first) { ymin=0; ymax=1; }
        if (std::abs(ymax-ymin) < 1e-12) { ymax += 0.5; ymin -= 0.5; }
        double pad = 0.05*(ymax-ymin);
        return {ymin - pad, ymax + pad};
    }

    // --- NEW: union Y range for every series that has the same unit (ignores axis)
    std::pair<double,double> visibleYRangeForUnit(const std::string& unit,
                                                  double tmin, double tmax) const {
        bool first = true; double ymin = 0.0, ymax = 0.0;
        for (const auto& s : series_) {
            if (s.unit != unit) continue;
            if (s.t.empty() || s.y.empty()) continue;
            size_t i0 = nearestIndex(s.t, tmin);
            size_t i1 = nearestIndex(s.t, tmax);
            if (i0 > i1) std::swap(i0, i1);
            i1 = std::min(i1 + 1, s.y.size() - 1);
            for (size_t i = i0; i <= i1; ++i) {
                if (first) { ymin = ymax = s.y[i]; first = false; }
                else { ymin = std::min(ymin, s.y[i]); ymax = std::max(ymax, s.y[i]); }
            }
        }
        if (first) { ymin = 0.0; ymax = 1.0; }
        if (std::abs(ymax - ymin) < 1e-12) { ymax += 0.5; ymin -= 0.5; }
        double pad = 0.05 * (ymax - ymin);
        return { ymin - pad, ymax + pad };
    }

    bool hasAxis(PlotSeries::Axis ax) const {
        for (const auto& s : series_) if (s.axis==ax) return true;
        return false;
    }
    std::string firstUnitOnAxis(PlotSeries::Axis ax) const {
        for (const auto& s : series_) if (s.axis==ax && !s.unit.empty()) return s.unit;
        return "";
    }
    std::string unitForAxis(PlotSeries::Axis ax) const {
        std::string u = firstUnitOnAxis(ax);
        return u.empty()? (ax==PlotSeries::Axis::Left? "Y (L)":"Y (R)") : u;
    }

    // ---------- draw helpers ----------
    static std::string fmt(double v) { char b[64]; std::snprintf(b, sizeof(b), "%.6g", v); return b; }
    static void drawText(SDL_Renderer* rr, TTF_Font* f, const std::string& s, int x, int y, SDL_Color c) {
        if (!f) return;
        SDL_Surface* surf = TTF_RenderUTF8_Blended(f, s.c_str(), c);
        if (!surf) return;
        SDL_Texture* tex = SDL_CreateTextureFromSurface(rr, surf);
        SDL_Rect dst{ x, y, surf->w, surf->h };
        SDL_FreeSurface(surf);
        if (tex) { SDL_RenderCopy(rr, tex, nullptr, &dst); SDL_DestroyTexture(tex); }
    }
    static void textSize(TTF_Font* f, const std::string& s, int& w, int& h) {
        if (!f) { w=0; h=0; return; }
        TTF_SizeUTF8(f, s.c_str(), &w, &h);
    }
    static void drawButton(SDL_Renderer* rr, TTF_Font* f, const SDL_Rect& rc, const char* label) {
        int mx,my; SDL_GetMouseState(&mx,&my);
        bool hover = (mx>=rc.x && mx<=rc.x+rc.w && my>=rc.y && my<=rc.y+rc.h);
        SDL_SetRenderDrawColor(rr, hover?70:60, hover?160:140, hover?240:220, 255);
        SDL_RenderFillRect(rr, &rc);
        SDL_SetRenderDrawColor(rr, 50,70,110,255);
        SDL_RenderDrawRect(rr, &rc);
        if (f && label) {
            SDL_Color c{255,255,255,255};
            SDL_Surface* s = TTF_RenderUTF8_Blended(f, label, c);
            if (s) {
                SDL_Texture* t = SDL_CreateTextureFromSurface(rr, s);
                SDL_Rect dst{ rc.x + (rc.w - s->w)/2, rc.y + (rc.h - s->h)/2, s->w, s->h };
                SDL_FreeSurface(s);
                if (t) { SDL_RenderCopy(rr, t, nullptr, &dst); SDL_DestroyTexture(t); }
            }
        }
    }
    void drawXTicks(double tmin, double tmax) {
        SDL_Rect p = plotRect(); int n = 5;
        for (int i=0;i<=n;++i) {
            double tt = tmin + (tmax - tmin) * (double)i / (double)n;
            int x = Vx(tt);
            SDL_SetRenderDrawColor(ren_, 210,215,230,255);
            SDL_RenderDrawLine(ren_, x, p.y, x, p.y+p.h);
            drawText(ren_, fAxis_, fmt(tt), x-16, p.y+p.h+8, SDL_Color{90,110,140,255});
        }
    }
    void drawYTicksLeft(double ymin, double ymax) {
        SDL_Rect p = plotRect(); int n = 5;
        for (int i=0;i<=n;++i) {
            double yy = ymin + (ymax - ymin) * (double)i / (double)n;
            int y = VyLeft(yy, ymin, ymax);
            SDL_SetRenderDrawColor(ren_, 225,230,240,255);
            SDL_RenderDrawLine(ren_, p.x, y, p.x+p.w, y);
            drawText(ren_, fAxis_, fmt(yy), 8, y-8, SDL_Color{90,110,140,255});
        }
    }
    void drawYTicksRight(double ymin, double ymax) {
        SDL_Rect p = plotRect(); int n = 5;
        for (int i=0;i<=n;++i) {
            double yy = ymin + (ymax - ymin) * (double)i / (double)n;
            int y = VyRight(yy, ymin, ymax);
            SDL_SetRenderDrawColor(ren_, 225,230,240,255);
            SDL_RenderDrawLine(ren_, p.x, y, p.x+p.w, y);
            std::string txt = fmt(yy);
            int tw=0,th=0; textSize(fAxis_, txt, tw, th);
            drawText(ren_, fAxis_, txt, p.x + p.w - tw - 6, y-8, SDL_Color{90,110,140,255});
        }
    }
    void drawSeries(const PlotSeries& s,
                    double tmin, double tmax, double ymin, double ymax, bool leftAxis)
    {
        SDL_SetRenderDrawColor(ren_, s.color.r, s.color.g, s.color.b, 255);
        size_t N = std::min(s.t.size(), s.y.size()); if (N < 2) return;
        size_t i0 = nearestIndex(s.t, tmin); if (i0>0) --i0;
        for (size_t i=i0+1; i<N; ++i) {
            double t0 = s.t[i-1], t1 = s.t[i];
            if (t1 < tmin) continue; if (t0 > tmax) break;
            int x0 = Vx(t0);
            int y0 = leftAxis ? VyLeft (s.y[i-1], ymin, ymax)
                              : VyRight(s.y[i-1], ymin, ymax);
            int x1 = Vx(t1);
            int y1 = leftAxis ? VyLeft (s.y[i],   ymin, ymax)
                              : VyRight(s.y[i],   ymin, ymax);
            SDL_RenderDrawLine(ren_, x0,y0, x1,y1);
        }
    }
    static void drawThickV(SDL_Renderer* rr, int y0, int y1, int x, SDL_Color c) {
        SDL_SetRenderDrawColor(rr, c.r, c.g, c.b, c.a);
        SDL_RenderDrawLine(rr, x,   y0, x,   y1);
        SDL_RenderDrawLine(rr, x-1, y0, x-1, y1);
        SDL_RenderDrawLine(rr, x+1, y0, x+1, y1);
    }
    static void drawThickH(SDL_Renderer* rr, int x0, int x1, int y, SDL_Color c) {
        SDL_SetRenderDrawColor(rr, c.r, c.g, c.b, c.a);
        SDL_RenderDrawLine(rr, x0, y, x1, y);
        SDL_RenderDrawLine(rr, x0, y-1, x1, y-1);
        SDL_RenderDrawLine(rr, x0, y+1, x1, y+1);
    }
    void drawPanel(const std::vector<std::string>& lines, int x, int y) {
        if (!fUI_ || lines.empty()) return;
        int wmax=0, hline=0;
        std::vector<SDL_Texture*> texs; std::vector<SDL_Rect> rects;
        for (const auto& ln : lines) {
            SDL_Color col{30,40,60,255};
            SDL_Surface* surf = TTF_RenderUTF8_Blended(fUI_, ln.c_str(), col);
            if (!surf) continue;
            wmax = std::max(wmax, surf->w); hline = std::max(hline, surf->h);
            SDL_Texture* t = SDL_CreateTextureFromSurface(ren_, surf);
            rects.push_back(SDL_Rect{0,0,surf->w,surf->h});
            SDL_FreeSurface(surf);
            texs.push_back(t);
        }
        int pad=8, gap=2, Htot = (int)texs.size()*hline + ((int)texs.size()-1)*gap + 2*pad;
        int Wtot = wmax + 2*pad;
        SDL_Rect p = plotRect();
        if (x + Wtot > p.x + p.w - 4) x = p.x + p.w - 4 - Wtot;
        if (y + Htot > p.y + p.h - 4) y = p.y + p.h - 4 - Htot;
        SDL_Rect box{ x, y, Wtot, Htot };
        SDL_SetRenderDrawColor(ren_, 255,255,255,235);
        SDL_RenderFillRect(ren_, &box);
        SDL_SetRenderDrawColor(ren_, 160,175,200,255);
        SDL_RenderDrawRect(ren_, &box);
        int cy = y + pad;
        for (size_t i=0;i<texs.size(); ++i) {
            if (!texs[i]) continue;
            SDL_Rect dst{ x + pad, cy, rects[i].w, rects[i].h };
            SDL_RenderCopy(ren_, texs[i], nullptr, &dst);
            SDL_DestroyTexture(texs[i]);
            cy += hline + gap;
        }
    }

    // ---------- legend ----------
    std::vector<SDL_Rect> buildLegendRectsTopRight(const SDL_Rect& plot) {
        std::vector<SDL_Rect> out; out.reserve(series_.size());
        int square = 18, gapY = 22, margin = 10;
        int xRight = plot.x + plot.w - margin;
        int y = plot.y + margin;
        for (size_t i=0;i<series_.size(); ++i) {
            SDL_Rect rc{ xRight - square, y, square, square };
            out.push_back(rc);
            y += gapY;
        }
        legendColorRects_ = out;
        return out;
    }
    void drawLegendTopRight(const SDL_Rect& plot, const std::vector<SDL_Rect>& colorRects) {
        if (!fUI_) return;
        for (size_t i=0;i<colorRects.size(); ++i) {
            const SDL_Rect& rc = colorRects[i];
            SDL_Color col = series_[i].color;
            SDL_SetRenderDrawColor(ren_, col.r, col.g, col.b, 255);
            SDL_RenderFillRect(ren_, &rc);
            SDL_SetRenderDrawColor(ren_, 40,60,90,255);
            SDL_RenderDrawRect(ren_, &rc);

            if ((int)i == activeSeries_) {
                SDL_SetRenderDrawColor(ren_, 255,180,40,255);
                SDL_Rect hl = rc; hl.x -= 2; hl.y -= 2; hl.w += 4; hl.h += 4;
                SDL_RenderDrawRect(ren_, &hl);
            }

            std::string tag = "#" + std::to_string(int(i+1)) + " " + series_[i].label;
            if (!series_[i].unit.empty()) tag += " (" + series_[i].unit + ")";
            tag += (series_[i].axis == PlotSeries::Axis::Left ? " [L]" : " [R]");
            int tw=0, th=0; textSize(fUI_, tag, tw, th);
            int lx = rc.x - 8 - tw;
            int ly = rc.y + (rc.h - th)/2;
            if (lx < plot.x + 6) lx = plot.x + 6;
            drawText(ren_, fUI_, tag, lx, ly, SDL_Color{40,60,90,255});
        }
    }
    int legendHit(const std::vector<SDL_Rect>& colorRects, int mx, int my) const {
        for (size_t i=0;i<colorRects.size(); ++i)
            if (inRect(colorRects[i], mx, my)) return (int)i;
        return -1;
    }

    // ---------- palette ----------
    SDL_Rect layoutPaletteRectTopRight(const SDL_Rect& plot,
                                       const std::vector<SDL_Rect>& legendRects) const
    {
        const int cols = 6, cell = 22, pad = 8;
        int rows = (int)std::ceil(presetColors_.size() / (double)cols);
        int w = cols*cell + 2*pad;
        int h = rows*cell + 2*pad + 18;

        int legendHeight = 0;
        if (!legendRects.empty()) {
            const SDL_Rect& last = legendRects.back();
            legendHeight = (last.y + last.h) - legendRects.front().y;
        }
        SDL_Rect rc; rc.w = w; rc.h = h;
        rc.x = plot.x + plot.w - 10 - w;
        rc.y = plot.y + 10 + legendHeight + 8;
        if (rc.y + rc.h > plot.y + plot.h - 8) rc.y = plot.y + plot.h - 8 - rc.h;
        return rc;
    }
    void drawPalette(const SDL_Rect& rc) {
        SDL_SetRenderDrawColor(ren_, 255,255,255,245);
        SDL_RenderFillRect(ren_, &rc);
        SDL_SetRenderDrawColor(ren_, 50,70,110,255);
        SDL_RenderDrawRect(ren_, &rc);
        drawText(ren_, fUI_, "Palette", rc.x + 8, rc.y + 4, SDL_Color{40,60,90,255});

        const int cols = 6, cell = 22, pad = 8;
        int x0 = rc.x + pad, y0 = rc.y + pad + 16;

        bool haveTarget = (paletteTargetIndex_>=0 && paletteTargetIndex_<(int)series_.size());
        SDL_Color cur{0,0,0,0};
        if (haveTarget) cur = series_[paletteTargetIndex_].color;

        for (size_t i=0;i<presetColors_.size(); ++i) {
            int r = int(i) / cols, c = int(i) % cols;
            SDL_Rect sw{ x0 + c*cell, y0 + r*cell, cell-2, cell-2 };
            SDL_Color col = presetColors_[i];
            SDL_SetRenderDrawColor(ren_, col.r, col.g, col.b, 255);
            SDL_RenderFillRect(ren_, &sw);
            SDL_SetRenderDrawColor(ren_, 60,60,60,255);
            SDL_RenderDrawRect(ren_, &sw);
            if (haveTarget && colorsEqual(col, cur)) {
                SDL_SetRenderDrawColor(ren_, 255,180,40,255);
                SDL_Rect hl = sw; hl.x -= 2; hl.y -= 2; hl.w += 4; hl.h += 4;
                SDL_RenderDrawRect(ren_, &hl);
            }
        }
    }
    int paletteIndexAt(const SDL_Rect& rc, int mx, int my) const {
        const int cols = 6, cell = 22, pad = 8;
        int x0 = rc.x + pad, y0 = rc.y + pad + 16;
        if (!inRect(rc, mx, my)) return -1;
        int lx = mx - x0, ly = my - y0; if (lx < 0 || ly < 0) return -1;
        int c = lx / cell, r = ly / cell; if (c < 0 || r < 0) return -1;
        int idx = r*cols + c; if (idx < 0 || idx >= (int)presetColors_.size()) return -1;
        int withinX = lx % cell, withinY = ly % cell;
        if (withinX >= cell-2 || withinY >= cell-2) return -1;
        return idx;
    }
    static bool colorsEqual(SDL_Color a, SDL_Color b) {
        return a.r==b.r && a.g==b.g && a.b==b.b && a.a==b.a;
    }

    // ---------- bottom “Combine signals” ----------
    void drawBottomBar() {
        SDL_Rect p = plotRect();
        SDL_SetRenderDrawColor(ren_, 200,205,220,255);
        SDL_RenderDrawLine(ren_, Lm_, p.y+p.h+14, W_-Rm_, p.y+p.h+14);

        SDL_Rect panel{ Lm_, p.y+p.h+18, W_-Lm_-Rm_, H_ - (p.y+p.h+18) - 10 };
        SDL_SetRenderDrawColor(ren_, 246,248,252,255);
        SDL_RenderFillRect(ren_, &panel);
        SDL_SetRenderDrawColor(ren_, 200,205,220,255);
        SDL_RenderDrawRect(ren_, &panel);

        SDL_Rect exprInput, exprAdd; layoutExprBar(exprInput, exprAdd);

        drawText(ren_, fUI_, "Combine signals:", panel.x + 10, panel.y + 10, SDL_Color{40,60,90,255});

        int mx,my; SDL_GetMouseState(&mx,&my);
        bool hover = inRect(exprInput, mx, my);
        SDL_SetRenderDrawColor(ren_, 255,255,255,255);
        SDL_RenderFillRect(ren_, &exprInput);
        SDL_SetRenderDrawColor(ren_, hover || exprActive_ ? 70:50, hover || exprActive_ ? 160:130, hover || exprActive_ ? 240:200, 255);
        SDL_RenderDrawRect(ren_, &exprInput);
        if (exprBuf_[0]) drawText(ren_, fUI_, exprBuf_, exprInput.x + 6, exprInput.y + 4, SDL_Color{30,40,60,255});
        else             drawText(ren_, fUI_, "e.g., #1 - #2   or   V(N001) + I(R1)", exprInput.x + 6, exprInput.y + 4, SDL_Color{130,140,160,255});

        drawButton(ren_, fUI_, exprAdd, "Add");

        std::string avail = "Available: ";
        for (size_t i=0;i<series_.size(); ++i) {
            if (i) avail += ", ";
            avail += "#" + std::to_string(int(i+1)) + " " + series_[i].label;
        }
        drawText(ren_, fUI_, avail, panel.x + 10, exprInput.y + exprInput.h + 8, SDL_Color{80,100,140,255});

        if (!exprError_.empty())
            drawText(ren_, fUI_, exprError_, exprInput.x, exprInput.y + exprInput.h + 28, SDL_Color{180,60,60,255});
    }
    void layoutExprBar(SDL_Rect& inputOut, SDL_Rect& addBtnOut) const {
        SDL_Rect p = plotRect();
        int y = p.y + p.h + 38;
        int inputH = 26;
        inputOut = SDL_Rect{ Lm_ + 150, y, W_ - (Lm_ + 150) - 110, inputH };
        addBtnOut = SDL_Rect{ inputOut.x + inputOut.w + 10, y, 80, inputH };
    }

    // ---------- expression engine ----------
    // resolve token to index: "#N", "N", "sN", or label
    int resolveTokenToIndex(const std::string& rawTok) const {
        if (rawTok.empty()) return -1;
        if (rawTok[0]=='#' || rawTok[0]=='s' || std::isdigit((unsigned char)rawTok[0])) {
            int n = -1;
            if (rawTok[0]=='#' || rawTok[0]=='s') { if (rawTok.size()>1 && std::isdigit((unsigned char)rawTok[1])) n = std::atoi(rawTok.c_str()+1); }
            else n = std::atoi(rawTok.c_str());
            if (n >= 1 && n <= (int)series_.size()) return n-1;
        }
        std::string want = normalizeKey(rawTok);
        for (size_t i=0;i<series_.size(); ++i) {
            if (normalizeKey(series_[i].label) == want) return (int)i;
            if (series_[i].key == want)                 return (int)i;
        }
        return -1;
    }
    // parse "A+B-C" -> (idx, sign) and a pretty label
    bool parseExpr(const char* expr, std::vector<std::pair<int,int>>& termsOut, std::string& prettyOut) {
        termsOut.clear(); prettyOut.clear();
        if (!expr) { exprError_ = "Empty expression."; return false; }

        std::string s; for (const char* p=expr; *p; ++p) if (!std::isspace((unsigned char)*p)) s.push_back(*p);
        if (s.empty()) { exprError_ = "Empty expression."; return false; }

        int sign = +1; std::string tok; std::vector<std::pair<std::string,int>> raw;
        for (char c : s) {
            if (c=='+' || c=='-') { if (!tok.empty()) { raw.push_back({tok,sign}); tok.clear(); } sign = (c=='+')? +1 : -1; }
            else tok.push_back(c);
        }
        if (!tok.empty()) raw.push_back({tok,sign});
        if (raw.empty()) { exprError_ = "No terms found."; return false; }

        for (auto& r : raw) {
            int idx = resolveTokenToIndex(r.first);
            if (idx < 0) { exprError_ = std::string("Unknown signal: ") + r.first; termsOut.clear(); return false; }
            termsOut.push_back({idx, r.second});
            if (!prettyOut.empty()) prettyOut += (r.second>0 ? " + " : " - ");
            else if (r.second<0)    prettyOut += "-";
            prettyOut += series_[idx].label;
        }
        return !termsOut.empty();
    }

    void commitExpression() {
        exprError_.clear();
        if (series_.empty()) { exprError_ = "No signals to combine."; return; }

        std::vector<std::pair<int,int>> terms;
        std::string pretty;
        if (!parseExpr(exprBuf_, terms, pretty)) return;

        // Compute from a stable snapshot so push_back cannot affect inputs.
        std::vector<PlotSeries> src = series_;
        int baseIdx = terms.front().first;
        const PlotSeries& base = src[baseIdx];
        if (base.t.empty()) { exprError_ = "Base signal has no data."; return; }

        std::vector<double> outY(base.t.size(), 0.0);
        std::string unitAll = src[baseIdx].unit;
        bool mixedUnit=false;

        for (size_t ti=0; ti<terms.size(); ++ti) {
            int idx = terms[ti].first, sgn = terms[ti].second;
            const PlotSeries& S = src[idx];
            if (idx != baseIdx && S.unit != unitAll) mixedUnit = true;
            for (size_t k=0; k<outY.size(); ++k)
                outY[k] += sgn * sampleAt(S, base.t[k]);  // onto base grid
        }

        PlotSeries out(pretty, base.t, outY,
                       presetColors_[ series_.size() % presetColors_.size() ]);
        out.key  = uniqueKey(std::string("expr_") + std::to_string(nextExprId_++));
        out.unit = mixedUnit ? "" : unitAll;

        // IMPORTANT: put expression on SAME axis as first operand if units match,
        // otherwise on the opposite axis (mixed units).
        out.axis = mixedUnit
                   ? (src[baseIdx].axis == PlotSeries::Axis::Left
                      ? PlotSeries::Axis::Right : PlotSeries::Axis::Left)
                   :  src[baseIdx].axis;

        series_.push_back(std::move(out));
        refreshGlobalTimeRange(false);

        // tidy input state
        exprActive_ = false; SDL_StopTextInput();
        exprBuf_[0] = 0;
    }


    // ---------- key helpers ----------
    static std::string normalizeKey(const std::string& s) {
        std::string out; out.reserve(s.size());
        for (char ch : s) {
            unsigned char c = (unsigned char)ch;
            if (std::isalnum(c) || c=='(' || c==')' || c=='_' )
                out.push_back((char)std::tolower(c));
        }
        return out;
    }
    std::string uniqueKey(const std::string& base) const {
        std::string k = base; int suffix = 1; bool clash = true;
        while (clash) {
            clash = false;
            for (const auto& s : series_) if (s.key == k) { clash = true; break; }
            if (clash) k = base + "_" + std::to_string(++suffix);
        }
        return k;
    }

private:
    // window
    std::string  title_;
    int          W_, H_;
    SDL_Window*  win_ = nullptr;
    SDL_Renderer*ren_ = nullptr;
    Uint32       windowID_ = 0;
    bool         open_ = false;

    // fonts
    TTF_Font* fTitle_ = nullptr;
    TTF_Font* fAxis_  = nullptr;
    TTF_Font* fUI_    = nullptr;

    // data
    std::vector<PlotSeries> series_;

    // layout (bottom bar space)
    int Lm_ = 60, Rm_ = 16, Tm_ = 42, Bm_ = 110;
    double gTmin_ = 0, gTmax_ = 1;
    double viewTmin_ = 0, viewTmax_ = 1;

    // panning
    bool   draggingPan_ = false;
    int    dragStartX_ = 0;
    double dragStartTmin_ = 0, dragStartTmax_ = 0;

    // UI state
    int  activeSeries_ = -1;
    bool showPalette_ = false;
    int  paletteTargetIndex_ = -1;
    SDL_Rect paletteRc_{0,0,0,0};
    std::vector<SDL_Rect> legendColorRects_;
    const std::vector<SDL_Color> presetColors_ = {
            { 30,130,220,255},{220, 80, 80,255},{ 80,170, 90,255},{200,160, 60,255},
            {140, 90,200,255},{  0,  0,  0,255},{ 90, 90, 90,255},{255,  0,  0,255},
            {  0,128,  0,255},{  0,  0,255,255},{255,128,  0,255},{128,  0,255,255},
            {  0,200,200,255},{200,  0,150,255},{150,100,  0,255},{  0,150, 60,255},
            { 60,  0,150,255},{150,  0, 60,255},{ 40,110,170,255},{170,110, 40,255},
            { 40,170,110,255},{110, 40,170,255},{170, 40,110,255},{110,170, 40,255}
    };

    // expression
    char exprBuf_[256];
    bool exprActive_ = false;
    std::string exprError_;
    int nextExprId_ = 1;
};



struct ButtonStyle {
    Uint8 fillR=40,  fillG=60,  fillB=90,  fillA=230;
    Uint8 fillOnR=70, fillOnG=160, fillOnB=240, fillOnA=255;
    Uint8 fillHotR=60, fillHotG=120, fillHotB=180, fillHotA=230;
    Uint8 borderR=60, borderG=20,  borderB=30,  borderA=255;
    int  radius = 8;
    int  padX = 12, padY = 7;
};

class Button {
public:
    using Callback = std::function<void(Button&)>;

    Button(std::string name, std::string label, SDL_Rect rect,
           bool toggleable=false, Callback onClick=nullptr)
            : name_(std::move(name)), label_(std::move(label)),
              rect_(rect), toggleable_(toggleable), onClick_(std::move(onClick)) {}

    // identity (can be edited later)
    const std::string& name()  const { return name_; }
    const std::string& label() const { return label_; }
    void setName(std::string s)  { name_ = std::move(s); }
    void setLabel(std::string s) { label_ = std::move(s); }

    // state
    bool visible = true;
    bool enabled = true;
    bool on = false; // for toggle buttons

    // layout
    SDL_Rect&       rect()       { return rect_; }
    const SDL_Rect& rect() const { return rect_; }

    void setCallback(Callback cb){ onClick_ = std::move(cb); }
    void setToggleable(bool t)   { toggleable_ = t; }

    // returns true if it consumed the event
    bool handleEvent(const SDL_Event& e, int mx, int my) {
        if (!visible || !enabled) return false;
        const bool inside = hit(mx,my);
        if (e.type==SDL_MOUSEBUTTONDOWN && e.button.button==SDL_BUTTON_LEFT && inside) {
            if (toggleable_) on = !on;
            if (onClick_)    onClick_(*this);
            return true;
        }
        return false;
    }

    void draw(SDL_Renderer* r, TTF_Font* font, const ButtonStyle& st = {}) const {
        if (!visible) return;
        int mx,my; SDL_GetMouseState(&mx,&my);
        const bool hot = hit(mx,my);

        Uint8 fr = on ? st.fillOnR : (hot ? st.fillHotR : st.fillR);
        Uint8 fg = on ? st.fillOnG : (hot ? st.fillHotG : st.fillG);
        Uint8 fb = on ? st.fillOnB : (hot ? st.fillHotB : st.fillB);
        Uint8 fa = on ? st.fillOnA : (hot ? st.fillHotA : st.fillA);

        roundedBoxRGBA(r, rect_.x, rect_.y, rect_.x+rect_.w, rect_.y+rect_.h,
                       st.radius, fr,fg,fb,fa);
        roundedRectangleRGBA(r, rect_.x, rect_.y, rect_.x+rect_.w, rect_.y+rect_.h,
                             st.radius, st.borderR,st.borderG,st.borderB,st.borderA);

        if (!font) return;
        SDL_Color textCol{255,255,255,255};
        SDL_Surface* surf = TTF_RenderUTF8_Blended(font, label_.c_str(), textCol);
        if (!surf) return;
        SDL_Texture* tex = SDL_CreateTextureFromSurface(r, surf);
        SDL_FreeSurface(surf);
        if (!tex) return;

        // Vertically center-ish: padY is tuned for your 36px button height
        SDL_Rect dst{ rect_.x + st.padX, rect_.y + st.padY, 0, 0 };
        SDL_QueryTexture(tex, nullptr, nullptr, &dst.w, &dst.h);
        SDL_RenderCopy(r, tex, nullptr, &dst);
        SDL_DestroyTexture(tex);
    }

private:
    bool hit(int x, int y) const {
        return x>=rect_.x && x<=rect_.x+rect_.w && y>=rect_.y && y<=rect_.y+rect_.h;
    }

    std::string name_;
    std::string label_;
    SDL_Rect    rect_;
    bool        toggleable_ = false;
    Callback    onClick_;
};

class ButtonRow {
public:
    Button& add(Button b) { buttons_.push_back(std::move(b)); return buttons_.back(); }

    bool handleEvent(const SDL_Event& e, int mx, int my) {
        for (auto& b : buttons_) if (b.handleEvent(e,mx,my)) return true;
        return false;
    }
    void draw(SDL_Renderer* r, TTF_Font* font, const ButtonStyle& st = {}) const {
        for (const auto& b : buttons_) b.draw(r, font, st);
    }
    Button* byName(const std::string& n) {
        for (auto& b : buttons_) if (b.name()==n) return &b;
        return nullptr;
    }

private:
    std::vector<Button> buttons_;
};



// -----------------------------
// Schematic Editor (with Return)
// -----------------------------
void ShowSchematicEditorWindow() {
    const int W = 1100, H = 700;
    const int PALETTE_H = 150;
    const int ITEM_W = 100, ITEM_H = 44;
    const int ITEM_PAD_X = 12, ITEM_PAD_Y = 10;
    const int ITEM_RADIUS = 10;
    const int NODE_S = 8;

    SDL_Window* win = SDL_CreateWindow(
            "New Project — Schematic Editor",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, W, H, SDL_WINDOW_SHOWN);
    if (!win) return;

    extern std::string gProjectPath;
    {
        std::string title = "Schematic Editor";
        if (!gProjectPath.empty()) {
            size_t pos = gProjectPath.find_last_of("\\/");
            std::string fname = (pos == std::string::npos) ? gProjectPath : gProjectPath.substr(pos + 1);
            title = fname;
        }
        SDL_SetWindowTitle(win, title.c_str());
    }

    SDL_Renderer* r = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!r) { SDL_DestroyWindow(win); return; }

    TTF_Font* fontTitle = loadUIFont(20);
    TTF_Font* fontBtn   = loadUIFont(18);

    // Top bar buttons
    SDL_Rect btnReturn{ W - 120, 12, 100, 36 };
    const int BTN_W=110, BTN_H=36, BTN_SP=10;
    SDL_Rect btnGND  { btnReturn.x - (BTN_SP + BTN_W)*1, 12, BTN_W, BTN_H };
    SDL_Rect btnTran { btnReturn.x - (BTN_SP + BTN_W)*2, 12, BTN_W, BTN_H };
    SDL_Rect btnAC   { btnReturn.x - (BTN_SP + BTN_W)*3, 12, BTN_W, BTN_H };
    SDL_Rect btnPhase{ btnReturn.x - (BTN_SP + BTN_W)*4, 12, BTN_W, BTN_H };
    bool groundPickMode=false;

    // Probes (appears after analysis)
    bool analysis_complete = false;
    TransientResult lastTR;

    enum class ProbeMode { None, V, I, P };
    ProbeMode probeMode = ProbeMode::None;

    SDL_Rect btnVProbe{ 16, 12, 110, 36 };
    SDL_Rect btnIProbe{ btnVProbe.x + btnVProbe.w + 8, 12, 110, 36 };
    SDL_Rect btnPProbe{ btnIProbe.x + btnIProbe.w + 8, 12, 110, 36 };

    ResultsWindow* resultsWin = nullptr;
    auto ensureResultsWinAndAdd = [&](const PlotSeries& s, const std::string& title = "Results"){
        if (!resultsWin || resultsWin->isClosed()) {
            if (resultsWin) { delete resultsWin; resultsWin = nullptr; }
            resultsWin = new ResultsWindow(title, 760, 460);
        }
        resultsWin->addSeries(s);
        resultsWin->bringToFront();
    };

    // Transient modal
    bool showTranDialog = false;
    int  tranFocus = 0; // 0=dt, 1=tstop
    static double last_dt = 1e-5, last_tstop = 1e-3;
    char bufDt[64], bufStop[64];
    std::string tranErr;

    // -------- AC SWEEP modal (new) --------
    bool showACDialog = false;
    int  acFocus = 0; // 0=fstart,1=fstop,2=npts,3=pts/period,4=settle,5=measure
    static double last_fstart = 10.0, last_fstop = 1e6;
    static int    last_npts = 50;
    static bool   last_logspace = true;
    static int    last_ppp = 200;
    static int    last_settle = 5;
    static int    last_measure = 1;
    char bufF1[64], bufF2[64], bufNpts[64], bufPPP[64], bufSettle[64], bufMeas[64];
    bool acLogspace = true;
    std::string acErr;

    auto fmtSci = [](double v){
        char b[64]; std::snprintf(b, sizeof(b), "%.8g", v);
        return std::string(b);
    };
    auto startTransientDialog = [&](){
        std::strncpy(bufDt,   fmtSci(last_dt).c_str(),   sizeof(bufDt));   bufDt[sizeof(bufDt)-1]=0;
        std::strncpy(bufStop, fmtSci(last_tstop).c_str(),sizeof(bufStop)); bufStop[sizeof(bufStop)-1]=0;
        showTranDialog = true; tranFocus = 0; tranErr.clear(); SDL_StartTextInput();
    };
    auto closeTransientDialog = [&](){ showTranDialog=false; SDL_StopTextInput(); };

    // AC dialog helpers (new)
    auto startACDialog = [&](){
        std::strncpy(bufF1,    fmtSci(last_fstart).c_str(), sizeof(bufF1));    bufF1[sizeof(bufF1)-1]=0;
        std::strncpy(bufF2,    fmtSci(last_fstop).c_str(),  sizeof(bufF2));    bufF2[sizeof(bufF2)-1]=0;
        std::snprintf(bufNpts, sizeof(bufNpts), "%d", last_npts);
        std::snprintf(bufPPP,  sizeof(bufPPP),  "%d", last_ppp);
        std::snprintf(bufSettle,sizeof(bufSettle),"%d", last_settle);
        std::snprintf(bufMeas, sizeof(bufMeas), "%d", last_measure);
        acLogspace = last_logspace;
        acErr.clear();
        showACDialog = true; acFocus = 0; SDL_StartTextInput();
    };
    auto closeACDialog = [&](){ showACDialog = false; SDL_StopTextInput(); };

    // Helper: draw text on light BG
    auto drawTextRGBA = [&](TTF_Font* f, const std::string& s, int x, int y,
                            Uint8 R, Uint8 G, Uint8 B, Uint8 A = 255)
    {
        SDL_Color col{R,G,B,A};
        SDL_Surface* surf = TTF_RenderUTF8_Blended(f, s.c_str(), col);
        if (!surf) return;
        SDL_Texture* tex = SDL_CreateTextureFromSurface(r, surf);
        SDL_Rect dst{ x, y, surf->w, surf->h };
        SDL_FreeSurface(surf);
        if (tex) { SDL_RenderCopy(r, tex, nullptr, &dst); SDL_DestroyTexture(tex); }
    };

    // Palette
    const char* passive[] = { "Resistor", "Capacitor", "Inductor", "Diode", "Zener" };
    const char* indepV[]  = { "Vsrc", "SINE_V", "PULSE_V", "DELTA_V" };
    const char* indepI[]  = { "Isrc", "SINE_I", "PULSE_I", "DELTA_I" };
    const char* dep[]     = { "VCVS", "VCCS", "CCVS", "CCCS" };

    std::vector<std::string> palette;
    palette.insert(palette.end(), std::begin(passive), std::end(passive));
    palette.insert(palette.end(), std::begin(indepV),  std::end(indepV));
    palette.insert(palette.end(), std::begin(indepI),  std::end(indepI));
    palette.insert(palette.end(), std::begin(dep),     std::end(dep));

    struct Item { SDL_Rect rect; std::string label; };
    std::vector<Item> palRects;

    auto buildPaletteLayout = [&](){
        palRects.clear();
        int rows = 2;
        int perRow = (int)std::ceil(palette.size()/double(rows));
        int x0=16, y0=H-PALETTE_H+20, idx=0;
        for (int row=0; row<rows; ++row) {
            int x=x0, y=y0 + row*(ITEM_H+ITEM_PAD_Y);
            for (int c=0; c<perRow && idx<(int)palette.size(); ++c, ++idx) {
                palRects.push_back({ SDL_Rect{ x, y, ITEM_W, ITEM_H }, palette[idx] });
                x += ITEM_W + ITEM_PAD_X;
            }
        }
    };
    buildPaletteLayout();

    struct Block { SDL_Rect rect; std::string type; std::string elem; };
    std::vector<Block> blocks;

    auto inside = [](const SDL_Rect& a, int x, int y){
        return x>=a.x && x<=a.x+a.w && y>=a.y && y<=a.y+a.h;
    };
    auto clamp = [&](int& x, int& y, const SDL_Rect& rc){
        const SDL_Rect canvas{ 0, 0, W, H - PALETTE_H };
        if (x < canvas.x) x = canvas.x;
        if (y < canvas.y) y = canvas.y;
        if (x + rc.w > canvas.x + canvas.w) x = canvas.x + canvas.w - rc.w;
        if (y + rc.h > canvas.y + canvas.h) y = canvas.y + canvas.h - rc.h;
    };

    // Colors
    auto colorForPalette = [&](const std::string& t, Uint8& R, Uint8& G, Uint8& B){
        if (t=="Resistor"){R=230;G=160;B=50;return;}
        if (t=="Capacitor"){R=80;G=190;B=220;return;}
        if (t=="Inductor"){R=100;G=200;B=120;return;}
        if (t=="Diode"||t=="Zener"){R=180;G=120;B=220;return;}
        if (t=="VCVS"||t=="VCCS"||t=="CCVS"||t=="CCCS"){R=255;G=120;B=140;return;}
        if (t.find("_V")!=std::string::npos||t=="Vsrc"){R=235;G=210;B=90;return;}
        if (t.find("_I")!=std::string::npos||t=="Isrc"){R=240;G=150;B=90;return;}
        R=190;G=190;B=200;
    };
    auto colorForBlock = [&](const Block& b, Uint8& R, Uint8& G, Uint8& B){
        Component* c = gCircuit.findComponent(b.elem);
        if (!c){ R=190;G=190;B=200; return; }
        if (dynamic_cast<VCVS*>(c) || dynamic_cast<VCCS*>(c) ||
            dynamic_cast<CCVS*>(c) || dynamic_cast<CCCS*>(c)) { R=255;G=120;B=140; return; }
        if (dynamic_cast<Resistor*>(c)){ R=230;G=160;B=50; return; }
        if (dynamic_cast<Capacitor*>(c)){ R=80;G=190;B=220; return; }
        if (dynamic_cast<Inductor*>(c)){ R=100;G=200;B=120; return; }
        if (dynamic_cast<Diode*>(c)){ R=180;G=120;B=220; return; }
        if (dynamic_cast<VoltageSource*>(c)){ R=235;G=210;B=90; return; }
        if (dynamic_cast<CurrentSource*>(c)){ R=240;G=150;B=90; return; }
        R=190;G=190;B=200;
    };
    auto isFourNodeBlock = [&](const Block& b)->bool{
        Component* c = gCircuit.findComponent(b.elem);
        return (dynamic_cast<VCVS*>(c) || dynamic_cast<VCCS*>(c) ||
                dynamic_cast<CCVS*>(c) || dynamic_cast<CCCS*>(c));
    };

    // Pin geometry
    auto pinRects = [&](const Block& b, std::array<SDL_Rect,4>& out, int& count){
        count = isFourNodeBlock(b) ? 4 : 2;
        if (count==2) {
            out[0] = SDL_Rect{ b.rect.x - NODE_S/2,            b.rect.y + b.rect.h/2 - NODE_S/2, NODE_S, NODE_S };
            out[1] = SDL_Rect{ b.rect.x + b.rect.w - NODE_S/2, b.rect.y + b.rect.h/2 - NODE_S/2, NODE_S, NODE_S };
        } else {
            out[0] = SDL_Rect{ b.rect.x - NODE_S/2,            b.rect.y + b.rect.h/4        - NODE_S/2, NODE_S, NODE_S }; // ctrl +
            out[1] = SDL_Rect{ b.rect.x - NODE_S/2,            b.rect.y + (3*b.rect.h)/4    - NODE_S/2, NODE_S, NODE_S }; // ctrl -
            out[2] = SDL_Rect{ b.rect.x + b.rect.w - NODE_S/2, b.rect.y + b.rect.h/4        - NODE_S/2, NODE_S, NODE_S }; // out +
            out[3] = SDL_Rect{ b.rect.x + b.rect.w - NODE_S/2, b.rect.y + (3*b.rect.h)/4    - NODE_S/2, NODE_S, NODE_S }; // out -
        }
    };
    auto pinCenter = [&](const SDL_Rect& pr)->SDL_Point{ return SDL_Point{ pr.x + pr.w/2, pr.y + pr.h/2 }; };

    // Hidden control VS helper
    auto nextNodeName = [&]()->std::string{
        int mx = 0;
        for (auto* n : gCircuit.nodes) {
            if (!n) continue;
            const std::string& s = n->name;
            if (!s.empty() && (s[0]=='N'||s[0]=='n')) {
                int v=0; for (size_t i=1;i<s.size() && isdigit((unsigned char)s[i]); ++i) v = v*10 + (s[i]-'0');
                if (v>mx) mx=v;
            }
        }
        int nxt = mx+1;
        std::ostringstream ss; ss << 'N' << std::setw(3) << std::setfill('0') << nxt;
        return ss.str();
    };
    auto ensureControlVS = [&](const std::string& ownerName)->VoltageSource*{
        std::string base = ownerName + "_CTRL";
        std::string name = base;
        if (auto* c = gCircuit.findComponent(name)) {
            if (auto* v = dynamic_cast<VoltageSource*>(c)) return v;
        }
        Node* a = gCircuit.addNode(nextNodeName());
        Node* b = gCircuit.addNode(nextNodeName());
        gCircuit.addVoltageSource(0.0, name, a, b);
        if (auto* v = dynamic_cast<VoltageSource*>(gCircuit.findComponent(name))) {
            v->IsMajaz = true; // hidden
            return v;
        }
        return nullptr;
    };

    // Map pin -> Node**
    auto nodePtrForPin = [&](const Block& b, int pinIdx)->Node**{
        Component* c = gCircuit.findComponent(b.elem);
        if (!c) return nullptr;

        auto resolve2 = [&](auto* obj)->Node**{
            if (!obj) return nullptr;
            return (pinIdx==0) ? &obj->Nude1 : &obj->Nude2;
        };

        if (!isFourNodeBlock(b)) {
            if (auto r = dynamic_cast<Resistor*>(c))         return resolve2(r);
            if (auto cp = dynamic_cast<Capacitor*>(c))       return resolve2(cp);
            if (auto l  = dynamic_cast<Inductor*>(c))        return resolve2(l);
            if (auto d  = dynamic_cast<Diode*>(c))           return resolve2(d);
            if (auto vs = dynamic_cast<VoltageSource*>(c))   return resolve2(vs);
            if (auto cs = dynamic_cast<CurrentSource*>(c))   return resolve2(cs);
            return nullptr;
        }

        if (auto e = dynamic_cast<VCVS*>(c)) {
            if      (pinIdx==0) return &e->ControlNude1;
            else if (pinIdx==1) return &e->ControlNude2;
            else if (pinIdx==2) return &e->Nude1;
            else if (pinIdx==3) return &e->Nude2;
            return nullptr;
        }
        if (auto g = dynamic_cast<VCCS*>(c)) {
            if      (pinIdx==0) return &g->ControlNude1;
            else if (pinIdx==1) return &g->ControlNude2;
            else if (pinIdx==2) return &g->Nude1;
            else if (pinIdx==3) return &g->Nude2;
            return nullptr;
        }
        if (auto h = dynamic_cast<CCVS*>(c)) {
            VoltageSource* ctrl = h->ControlVoltageSource;
            if (!ctrl) { ctrl = ensureControlVS(h->name); h->ControlVoltageSource = ctrl; }
            if (!ctrl) return nullptr;
            if      (pinIdx==0) return &ctrl->Nude1;
            else if (pinIdx==1) return &ctrl->Nude2;
            else if (pinIdx==2) return &h->Nude1;
            else if (pinIdx==3) return &h->Nude2;
            return nullptr;
        }
        if (auto f = dynamic_cast<CCCS*>(c)) {
            VoltageSource* ctrl = f->ControlVoltageSource;
            if (!ctrl) { ctrl = ensureControlVS(f->name); f->ControlVoltageSource = ctrl; }
            if (!ctrl) return nullptr;
            if      (pinIdx==0) return &ctrl->Nude1;
            else if (pinIdx==1) return &ctrl->Nude2;
            else if (pinIdx==2) return &f->Nude1;
            else if (pinIdx==3) return &f->Nude2;
            return nullptr;
        }
        return nullptr;
    };

    // Transient runner
    auto runTransientAndPrint = [&](double dt, double tstop){
        if (!gProjectPath.empty()) save_circuit_to_file(gProjectPath, gCircuit);

        auto findExistingGround = [&]()->Node*{
            for (auto* n : gCircuit.nodes) {
                if (!n) continue;
                if (n->IsG) return n;
                std::string s = n->name; for (auto& ch : s) ch = (char)std::toupper((unsigned char)ch);
                if (s=="0" || s=="GND" || s=="GROUND") return n;
            }
            return nullptr;
        };
        auto pickFallbackGround = [&]()->Node*{
            Node* best=nullptr;
            for (auto* n : gCircuit.nodes) {
                if (!n || n->IsMajaz) continue;
                if (!best) best=n;
                else if (n->name < best->name) best=n;
            }
            return best;
        };

        bool tempGround=false;
        Node* gnd = findExistingGround();
        if (!gnd) {
            gnd = pickFallbackGround();
            if (!gnd) { std::cout<<"[Transient] No nodes in circuit.\n"; return; }
            gnd->IsG = true; tempGround = true;
        }

        TransientResult R;
        bool ok = SolveTransient_Phase1(gCircuit, dt, tstop, R);

        if (tempGround && gnd) gnd->IsG = false;

        if (!ok) { std::cout << "[Transient] Solver failed.\n"; return; }

        lastTR = std::move(R);
        analysis_complete = true;
        probeMode = ProbeMode::None;
    };

    // -------- AC SWEEP runner (new) --------
    auto runACSweepAndPlot = [&](double f1, double f2, int npts, bool logspace, int ppp, int settle, int meas){
        if (!gProjectPath.empty()) save_circuit_to_file(gProjectPath, gCircuit);

        auto findExistingGround = [&]()->Node*{
            for (auto* n : gCircuit.nodes) {
                if (!n) continue;
                if (n->IsG) return n;
                std::string s = n->name; for (auto& ch : s) ch = (char)std::toupper((unsigned char)ch);
                if (s=="0" || s=="GND" || s=="GROUND") return n;
            }
            return nullptr;
        };
        auto pickFallbackGround = [&]()->Node*{
            Node* best=nullptr;
            for (auto* n : gCircuit.nodes) {
                if (!n || n->IsMajaz) continue;
                if (!best) best=n;
                else if (n->name < best->name) best=n;
            }
            return best;
        };

        bool tempGround=false;
        Node* gnd = findExistingGround();
        if (!gnd) {
            gnd = pickFallbackGround();
            if (!gnd) { std::cout<<"[AC Sweep] No nodes in circuit.\n"; return; }
            gnd->IsG = true; tempGround = true;
        }

        ACSweepResult AC;
        bool ok = SolveAC_Sweep_Phase1(gCircuit, f1, f2, npts, logspace, ppp, settle, meas, AC);

        if (tempGround && gnd) gnd->IsG = false;

        if (!ok) { std::cout << "[AC Sweep] Solver failed.\n"; return; }

        // Plot: Bode-like — |V| in dB (left), phase in deg (right) for each node (non-ground)
        // Clear/open results window titled "AC Sweep"
        if (resultsWin) { delete resultsWin; resultsWin = nullptr; }
        // magnitudes
        for (size_t i=0;i<AC.nodeNames.size();++i) {
            const std::string& nm = AC.nodeNames[i];
            // skip ground if present in list
            bool isG = false;
            if (Node* n = gCircuit.findNode(nm)) isG = n->IsG;
            if (isG) continue;

            std::vector<double> magdB(AC.freq.size()), phdeg(AC.freq.size());
            for (size_t k=0;k<AC.freq.size();++k) {
                double m = std::max(AC.Vmag[i][k], 1e-15);
                magdB[k] = 20.0*std::log10(m);
                phdeg[k] = AC.Vphase[i][k] * 180.0 / M_PI;
            }
            PlotSeries sMag("V("+nm+") [dB]", AC.freq, magdB, SDL_Color{120,200,255,255});
            sMag.unit = "dB"; sMag.axis = PlotSeries::Axis::Left;
            ensureResultsWinAndAdd(sMag, "AC Sweep");

            PlotSeries sPh("∠V("+nm+") [deg]", AC.freq, phdeg, SDL_Color{220,140,80,255});
            sPh.unit = "deg"; sPh.axis = PlotSeries::Axis::Right;
            ensureResultsWinAndAdd(sPh, "AC Sweep");
        }

        // you could also plot element currents (Imag/Iphase) similarly if desired

        // Bring results to front
        if (resultsWin) resultsWin->bringToFront();
    };

    // Visual wires
    struct Wire { std::string aComp; int aPin; std::string bComp; int bPin; };
    std::vector<Wire> wires;

    auto rebuildWiresFromNodes = [&](){
        wires.clear();
        struct Endp { std::string comp; int pin; };
        std::unordered_map<Node*, std::vector<Endp>> taps;

        for (const auto& b : blocks) {
            std::array<SDL_Rect,4> prs; int cnt=0; pinRects(b, prs, cnt);
            for (int pi=0; pi<cnt; ++pi) {
                if (Node** P = nodePtrForPin(b, pi)) {
                    Node* n = *P; if (!n) continue;
                    taps[n].push_back( Endp{ b.elem, pi } );
                }
            }
        }

        std::unordered_set<std::string> seen;
        auto ek = [](const Endp& A, const Endp& B){
            if (A.comp < B.comp || (A.comp==B.comp && A.pin < B.pin))
                return A.comp + "|" + std::to_string(A.pin) + "->" + B.comp + "|" + std::to_string(B.pin);
            return B.comp + "|" + std::to_string(B.pin) + "->" + A.comp + "|" + std::to_string(A.pin);
        };
        for (auto& kv : taps) {
            auto& v = kv.second;
            if (v.size() < 2) continue;
            const Endp& root = v[0];
            for (size_t i=1;i<v.size();++i) {
                std::string key = ek(root, v[i]);
                if (seen.insert(key).second)
                    wires.push_back( Wire{ root.comp, root.pin, v[i].comp, v[i].pin } );
            }
        }
    };

    struct PendingPin { bool active=false; std::string comp; int pin=-1; SDL_Point p{0,0}; };
    PendingPin pending;

    Uint32 lastClickTicks = 0;
    int    lastClickBlock = -1;

    // Separate double-click detector for pin squares
    Uint32 lastPinClickTicks = 0;
    int    lastPinBlock = -1;
    int    lastPinIndex = -1;

    std::string brush;

    bool dragging=false; int dragIdx=-1; int dragDX=0, dragDY=0;

    // ------ Combined Element Edit Dialog (rename + params + delete) ------
    bool showElemDialog = false;
    struct ElemDlgCtx {
        std::string oldName;
        Component* comp = nullptr;
        char nameBuf[128]{};
        std::string err;
        int ownerBlockIndex = -1;
    } elemDlg;

    auto openElementDialog = [&](const std::string& name, int blockIndex){
        Component* c = gCircuit.findComponent(name);
        if (!c) return;
        elemDlg.comp = c;
        elemDlg.oldName = name;
        elemDlg.ownerBlockIndex = blockIndex;
        std::strncpy(elemDlg.nameBuf, name.c_str(), sizeof(elemDlg.nameBuf)-1);
        elemDlg.nameBuf[sizeof(elemDlg.nameBuf)-1]=0;
        elemDlg.err.clear();
        showElemDialog = true;
        SDL_StartTextInput();
    };
    auto closeElementDialog = [&](){
        showElemDialog = false;
        SDL_StopTextInput();
    };
    auto componentNameExists = [&](const std::string& nm)->bool{
        if (nm == elemDlg.oldName) return false;
        return gCircuit.findComponent(nm) != nullptr;
    };
    auto renameHiddenControlIfNeeded = [&](Component* c, const std::string& oldN, const std::string& newN){
        if (auto h = dynamic_cast<CCVS*>(c)) {
            if (h->ControlVoltageSource && h->ControlVoltageSource->IsMajaz) {
                if (h->ControlVoltageSource->name == oldN + "_CTRL")
                    h->ControlVoltageSource->name = newN + "_CTRL";
            }
        }
        if (auto f = dynamic_cast<CCCS*>(c)) {
            if (f->ControlVoltageSource && f->ControlVoltageSource->IsMajaz) {
                if (f->ControlVoltageSource->name == oldN + "_CTRL")
                    f->ControlVoltageSource->name = newN + "_CTRL";
            }
        }
    };
    auto applyElementRename = [&]()->bool{
        std::string newName = elemDlg.nameBuf;
        // trim
        auto trim=[&](std::string& s){
            while(!s.empty() && std::isspace((unsigned char)s.front())) s.erase(s.begin());
            while(!s.empty() && std::isspace((unsigned char)s.back()))  s.pop_back();
        };
        trim(newName);
        if (newName.empty()) { elemDlg.err = "Name cannot be empty."; return false; }
        // forbid slashes
        for (char& ch : newName) if (ch=='/'||ch=='\\') ch='_' ;

        if (componentNameExists(newName)) {
            elemDlg.err = "Name already in use.";
            return false;
        }

        // carry UI rect to new name
        int x=0,y=0,w0=ITEM_W,h0=ITEM_H;
        (void)ui_get_element_rect(elemDlg.oldName, x,y,w0,h0);
        ui_set_element_position(newName, x,y,w0,h0);

        // rename component
        elemDlg.comp->name = newName;
        renameHiddenControlIfNeeded(elemDlg.comp, elemDlg.oldName, newName);

        // update block entry
        if (elemDlg.ownerBlockIndex>=0 && elemDlg.ownerBlockIndex < (int)blocks.size()) {
            blocks[elemDlg.ownerBlockIndex].elem = newName;
            blocks[elemDlg.ownerBlockIndex].type = UITypeForComponent(elemDlg.comp);
        } else {
            for (auto& b: blocks) if (b.elem == elemDlg.oldName) { b.elem = newName; b.type = UITypeForComponent(elemDlg.comp); }
        }

        elemDlg.oldName = newName; // update context
        return true;
    };
    auto deleteElement = [&](){
        if (!elemDlg.comp) return;
        elemDlg.comp->IsMajaz = true; // hide/remove
        // remove block
        if (elemDlg.ownerBlockIndex>=0 && elemDlg.ownerBlockIndex < (int)blocks.size()) {
            blocks.erase(blocks.begin()+elemDlg.ownerBlockIndex);
        } else {
            for (size_t i=0;i<blocks.size();++i) if (blocks[i].elem == elemDlg.comp->name) { blocks.erase(blocks.begin()+i); break; }
        }
        rebuildWiresFromNodes();
        if (!gProjectPath.empty()) save_circuit_to_file(gProjectPath, gCircuit);
        closeElementDialog();
    };

    auto editComponentByName = [&](const std::string& name, int blockIndex){
        // open our combined dialog instead of direct parameter editor
        openElementDialog(name, blockIndex);
    };

    // ------ Node rename dialog (per-pin) ------
    bool showNodeDialog = false;
    struct NodeDlgCtx {
        Node* original = nullptr;
        Node** targetPtr = nullptr;
        char buf[64]{};
        bool wantGround = false;
    } nodeDlg;

    auto startNodeDialog = [&](Node** nodePtr){
        if (!nodePtr || !(*nodePtr)) return;
        nodeDlg.original   = *nodePtr;
        nodeDlg.targetPtr  = nodePtr;
        std::string nm = nodeDlg.original->name.empty() ? nextNodeName() : nodeDlg.original->name;
        std::strncpy(nodeDlg.buf, nm.c_str(), sizeof(nodeDlg.buf)-1);
        nodeDlg.buf[sizeof(nodeDlg.buf)-1] = 0;
        nodeDlg.wantGround = nodeDlg.original->IsG;
        showNodeDialog = true;
        SDL_StartTextInput();
    };
    auto closeNodeDialog = [&](){ showNodeDialog = false; SDL_StopTextInput(); };
    auto applyNodeDialog = [&](){
        if (!nodeDlg.targetPtr) { closeNodeDialog(); return; }
        std::string newName = std::string(nodeDlg.buf);
        auto ltrim=[&](std::string& s){ while(!s.empty() && std::isspace((unsigned char)s.front())) s.erase(s.begin()); };
        auto rtrim=[&](std::string& s){ while(!s.empty() && std::isspace((unsigned char)s.back()))  s.pop_back(); };
        ltrim(newName); rtrim(newName);
        if (newName.empty()) newName = nextNodeName();

        Node* newNode = gCircuit.findNode(newName);
        if (!newNode) newNode = gCircuit.addNode(newName);

        *(nodeDlg.targetPtr) = newNode;

        if (nodeDlg.wantGround) {
            for (auto* n : gCircuit.nodes) if (n) n->IsG = false;
            newNode->IsG = true;
        } else {
            newNode->IsG = false;
        }

        rebuildWiresFromNodes();
        if (!gProjectPath.empty()) save_circuit_to_file(gProjectPath, gCircuit);
        closeNodeDialog();
    };

    // ------ Return Menu (rename/save options) ------
    bool showReturnMenu = false;
    char projectNameBuf[256]{};
    auto buildProjectNameBufFromPath = [&](){
        std::string fname = gProjectPath;
        size_t pos = fname.find_last_of("\\/");
        if (pos != std::string::npos) fname = fname.substr(pos+1);
        if (fname.size() > 4 && fname.substr(fname.size()-4)==".txt")
            fname = fname.substr(0, fname.size()-4);
        std::strncpy(projectNameBuf, fname.c_str(), sizeof(projectNameBuf)-1);
        projectNameBuf[sizeof(projectNameBuf)-1]=0;
    };

    // Load project once
    if (!gProjectPath.empty() && gCircuit.components.empty()) {
        read_circuit_from_file(gProjectPath, gCircuit);
    }

    // Build blocks from model
    {
        blocks.clear();
        std::unordered_set<std::string> seen;
        int fallbackX = 60, fallbackY = 80, stepX = 180, stepY = 120, col = 0;

        for (const auto* comp : gCircuit.components) {
            if (!comp || comp->IsMajaz) continue;
            if (!seen.insert(comp->name).second) continue;

            SDL_Rect rc{0,0,ITEM_W,ITEM_H};
            auto it = gUiPos.find(comp->name);
            if (it != gUiPos.end()) {
                rc.x = it->second.x; rc.y = it->second.y;
                rc.w = it->second.w; rc.h = it->second.h;
            } else {
                rc.x = fallbackX + col*stepX;
                rc.y = fallbackY;
                col = (col+1) % 6;
                if (col == 0) fallbackY += stepY;
                ui_set_element_position(comp->name, rc.x, rc.y, rc.w, rc.h);
            }
            blocks.push_back(Block{ rc, UITypeForComponent(comp), comp->name });
        }
        rebuildWiresFromNodes();
    }

    auto drawBtn = [&](const SDL_Rect& rc, const char* label, bool on=false){
        int mx,my; SDL_GetMouseState(&mx,&my);
        bool h = inside(rc, mx, my);
        Uint8 fillR = on? 70 : (h? 60: 40);
        Uint8 fillG = on?160 : (h? 80: 60);
        Uint8 fillB = on?240 : (h?120: 90);
        roundedBoxRGBA(r, rc.x, rc.y, rc.x+rc.w, rc.y+rc.h, 8, fillR,fillG,fillB, on?255:230);
        roundedRectangleRGBA(r, rc.x, rc.y, rc.x+rc.w, rc.y+rc.h, 8, 60,20,30,255);
        drawText(r, fontBtn, label, rc.x+12, rc.y+7, SDL_Color{255,255,255,255});
    };

    auto drawTextFieldCard = [&](const SDL_Rect& rc, const char* text, bool focused){
        Uint8 fr = focused ? 90 : 60, fg = focused ? 180 : 120, fb = focused ? 250 : 180;
        roundedBoxRGBA(r, rc.x, rc.y, rc.x+rc.w, rc.y+rc.h, 6, 245,245,250, 255);
        roundedRectangleRGBA(r, rc.x, rc.y, rc.x+rc.w, rc.y+rc.h, 6, fr,fg,fb,255);
        drawTextRGBA(fontBtn, text ? text : "", rc.x+8, rc.y+8, 40,60,90);
    };

    auto plotNodeVoltage = [&](Node* n){
        if (!n) return;
        auto it = lastTR.nodeIndex.find(n->name);
        if (it == lastTR.nodeIndex.end()) { std::cout << "[Probe] Node not in results.\n"; return; }
        size_t idx = it->second;
        PlotSeries s("V(" + n->name + ")", lastTR.t, lastTR.V[idx], SDL_Color{120,200,255,255});
        s.unit = "V"; s.axis = PlotSeries::Axis::Left;
        ensureResultsWinAndAdd(s, "Voltage/Current/Power");
    };
    auto plotElementCurrent = [&](const std::string& elemName){
        auto it = lastTR.elemIndexI.find(elemName);
        if (it == lastTR.elemIndexI.end()) { std::cout << "[Probe] Element I not available.\n"; return; }
        size_t idx = it->second;
        PlotSeries s("I(" + elemName + ")", lastTR.t, lastTR.I[idx], SDL_Color{220,80,80,255});
        s.unit = "A"; s.axis = PlotSeries::Axis::Right;
        ensureResultsWinAndAdd(s, "Voltage/Current/Power");
    };
    auto plotElementPower = [&](const std::string& elemName){
        Component* c = gCircuit.findComponent(elemName);
        if (!c) return;
        Node *n1=nullptr, *n2=nullptr;
        if (auto r0=dynamic_cast<Resistor*>(c)) { n1=r0->Nude1; n2=r0->Nude2; }
        else if (auto cp=dynamic_cast<Capacitor*>(c)) { n1=cp->Nude1; n2=cp->Nude2; }
        else if (auto L =dynamic_cast<Inductor*>(c)) { n1=L->Nude1; n2=L->Nude2; }
        else if (auto v =dynamic_cast<VoltageSource*>(c)) { n1=v->Nude1; n2=v->Nude2; }
        else if (auto i =dynamic_cast<CurrentSource*>(c)) { n1=i->Nude1; n2=i->Nude2; }
        else if (auto e =dynamic_cast<VCVS*>(c)) { n1=e->Nude1; n2=e->Nude2; }
        else if (auto g =dynamic_cast<VCCS*>(c)) { n1=g->Nude1; n2=g->Nude2; }
        else { std::cout << "[Probe] Power not supported.\n"; return; }

        auto itI = lastTR.elemIndexI.find(elemName);
        if (itI == lastTR.elemIndexI.end()) { std::cout << "[Probe] Element I missing.\n"; return; }
        auto itN1 = lastTR.nodeIndex.find(n1?n1->name:"");
        auto itN2 = lastTR.nodeIndex.find(n2?n2->name:"");
        if (itN1==lastTR.nodeIndex.end() || itN2==lastTR.nodeIndex.end()) { std::cout << "[Probe] V nodes missing.\n"; return; }
        size_t idxI = itI->second, i1 = itN1->second, i2 = itN2->second;

        std::vector<double> P(lastTR.t.size(), 0.0);
        for (size_t k=0;k<P.size();++k) {
            double v = lastTR.V[i1][k] - lastTR.V[i2][k];
            P[k] = v * lastTR.I[idxI][k];
        }
        PlotSeries s("P(" + elemName + ")", lastTR.t, P, SDL_Color{200,160,60,255});
        s.unit = "W"; s.axis = PlotSeries::Axis::Right;
        ensureResultsWinAndAdd(s, "Voltage/Current/Power");
    };

    auto enterProbe = [&](ProbeMode m){
        probeMode = (probeMode == m ? ProbeMode::None : m);
        pending.active = false;
        brush.clear();
        dragging=false;
        groundPickMode=false;
    };

    // Main loop
    bool running=true;
    while (running) {
        SDL_Event e; int mx=0,my=0; SDL_GetMouseState(&mx,&my);

        while (SDL_PollEvent(&e)) {
            // Forward to results window
            if (resultsWin && resultsWin->isOpen()) {
                resultsWin->handleEvent(e);
                if (resultsWin->isClosed()) { delete resultsWin; resultsWin = nullptr; }
            }

            if (e.type==SDL_QUIT) {
                if (!gProjectPath.empty()) save_circuit_to_file(gProjectPath, gCircuit);
                ResetModelsAndUI(); ResetCounters(); blocks.clear();
                running=false;
            }
            if (e.type==SDL_KEYDOWN && e.key.keysym.sym==SDLK_ESCAPE) {
                if (showTranDialog) { closeTransientDialog(); }
                else if (showACDialog) { closeACDialog(); } // AC modal close
                else if (showNodeDialog) { closeNodeDialog(); }
                else if (showElemDialog) { closeElementDialog(); }
                else if (probeMode != ProbeMode::None) { probeMode = ProbeMode::None; pending.active=false; }
                else if (pending.active) pending.active=false;
                else if (groundPickMode) groundPickMode=false;
                else if (showReturnMenu) { showReturnMenu=false; SDL_StopTextInput(); }
                else {
                    if (!gProjectPath.empty()) save_circuit_to_file(gProjectPath, gCircuit);
                    ResetModelsAndUI(); ResetCounters(); blocks.clear();
                    running=false;
                }
            }
            if (e.type==SDL_MOUSEBUTTONDOWN && e.button.button==SDL_BUTTON_RIGHT) {
                if (!showTranDialog && !showACDialog && !showNodeDialog && !showElemDialog && !showReturnMenu) { pending.active=false; groundPickMode=false; probeMode=ProbeMode::None; }
            }

            // --- Modal: Element dialog (rename + params + delete)
            if (showElemDialog) {
                if (e.type==SDL_MOUSEBUTTONDOWN && e.button.button==SDL_BUTTON_LEFT) {
                    SDL_Rect dlg{ W/2-300, H/2-140, 600, 280 };
                    SDL_Rect txtName{ dlg.x+24, dlg.y+58,  360, 36 };
                    SDL_Rect btnParam{ dlg.x+24, dlg.y+110, 140, 32 };
                    SDL_Rect btnSave{ dlg.x+dlg.w-320, dlg.y+dlg.h-56, 140, 36 };
                    SDL_Rect btnBack{ dlg.x+dlg.w-160, dlg.y+dlg.h-56, 140, 36 };
                    SDL_Rect btnDelete{ dlg.x+dlg.w-320, dlg.y+dlg.h-100, 276, 36 };

                    auto in=[&](SDL_Rect a){ return mx>=a.x && mx<=a.x+a.w && my>=a.y && my<=a.y+a.h; };

                    if (in(btnParam)) {
                        if (elemDlg.comp) {
                            // open your existing parameter editor
                            EditElementParameters(elemDlg.comp);
                            if (!gProjectPath.empty()) save_circuit_to_file(gProjectPath, gCircuit);
                        }
                    } else if (in(btnSave)) {
                        if (applyElementRename()) {
                            if (!gProjectPath.empty()) save_circuit_to_file(gProjectPath, gCircuit);
                            closeElementDialog();
                        }
                    } else if (in(btnBack)) {
                        closeElementDialog();
                    } else if (in(btnDelete)) {
                        deleteElement();
                    } else if (in(txtName)) {
                        // keep focus; text goes to nameBuf
                    }
                }
                if (e.type==SDL_TEXTINPUT) {
                    size_t L = strlen(elemDlg.nameBuf);
                    if (L < sizeof(elemDlg.nameBuf)-1) { strcat(elemDlg.nameBuf, e.text.text); }
                }
                if (e.type==SDL_KEYDOWN) {
                    if (e.key.keysym.sym==SDLK_BACKSPACE) {
                        size_t L = strlen(elemDlg.nameBuf);
                        if (L>0) elemDlg.nameBuf[L-1]=0;
                    }
                    if (e.key.keysym.sym==SDLK_RETURN) {
                        if (applyElementRename()) {
                            if (!gProjectPath.empty()) save_circuit_to_file(gProjectPath, gCircuit);
                            closeElementDialog();
                        }
                    }
                }
                continue;
            }

            // --- Modal: Node rename ---
            if (showNodeDialog) {
                if (e.type==SDL_MOUSEBUTTONDOWN && e.button.button==SDL_BUTTON_LEFT) {
                    SDL_Rect dlg{ W/2-240, H/2-110, 480, 220 };
                    SDL_Rect txtName{ dlg.x+24, dlg.y+56,  280, 36 };
                    SDL_Rect chkGnd { dlg.x+24, dlg.y+104,  18, 18 };
                    SDL_Rect btnOk{ dlg.x+dlg.w-200, dlg.y+dlg.h-56, 84, 32 };
                    SDL_Rect btnCancel{ dlg.x+dlg.w-104, dlg.y+dlg.h-56, 84, 32 };

                    auto in=[&](SDL_Rect a){return mx>=a.x && mx<=a.x+a.w && my>=a.y && my<=a.y+a.h;};
                    if (in(txtName)) { /* focus */ }
                    else if (in(chkGnd)) { nodeDlg.wantGround = !nodeDlg.wantGround; }
                    else if (in(btnOk)) { applyNodeDialog(); }
                    else if (in(btnCancel)) { closeNodeDialog(); }
                }
                if (e.type==SDL_TEXTINPUT) {
                    size_t L = strlen(nodeDlg.buf);
                    if (L < sizeof(nodeDlg.buf)-1) { strcat(nodeDlg.buf, e.text.text); }
                }
                if (e.type==SDL_KEYDOWN) {
                    if (e.key.keysym.sym==SDLK_BACKSPACE) {
                        size_t L = strlen(nodeDlg.buf);
                        if (L>0) nodeDlg.buf[L-1]=0;
                    }
                    if (e.key.keysym.sym==SDLK_RETURN) { applyNodeDialog(); }
                }
                continue;
            }

            // --- Modal: Transient ---
            if (showTranDialog) {
                if (e.type==SDL_MOUSEBUTTONDOWN && e.button.button==SDL_BUTTON_LEFT) {
                    SDL_Rect dlg{ W/2-260, H/2-120, 520, 240 };
                    SDL_Rect txtDt{ dlg.x+120, dlg.y+56,  160, 36 };
                    SDL_Rect txtSt{ dlg.x+396, dlg.y+56,  100, 36 };
                    SDL_Rect btnOk{ dlg.x+dlg.w-200, dlg.y+dlg.h-56, 84, 32 };
                    SDL_Rect btnCancel{ dlg.x+dlg.w-104, dlg.y+dlg.h-56, 84, 32 };

                    auto in=[&](SDL_Rect a){return mx>=a.x && mx<=a.x+a.w && my>=a.y && my<=a.y+a.h;};
                    if (in(txtDt)) tranFocus=0; else
                    if (in(txtSt)) tranFocus=1; else
                    if (in(btnOk)) {
                        double dt=0,tstop=0;
                        try{ dt=std::stod(bufDt); tstop=std::stod(bufStop);}catch(...){dt=0;tstop=0;}
                        if (dt<=0 || tstop<=0) { tranErr="Enter positive dt and tstop"; }
                        else {
                            last_dt = dt; last_tstop = tstop;
                            closeTransientDialog();
                            runTransientAndPrint(dt,tstop);
                        }
                    } else
                    if (in(btnCancel)) { closeTransientDialog(); }
                }
                if (e.type==SDL_TEXTINPUT) {
                    char* buf = (tranFocus==0)? bufDt : bufStop;
                    size_t L = strlen(buf);
                    if (L < 60) { strcat(buf, e.text.text); }
                }
                if (e.type==SDL_KEYDOWN) {
                    if (e.key.keysym.sym==SDLK_BACKSPACE) {
                        char* buf = (tranFocus==0)? bufDt : bufStop;
                        size_t L = strlen(buf);
                        if (L>0) buf[L-1]=0;
                    }
                    if (e.key.keysym.sym==SDLK_TAB) tranFocus = (tranFocus==0?1:0);
                    if (e.key.keysym.sym==SDLK_RETURN) {
                        double dt=0,tstop=0;
                        try{ dt=std::stod(bufDt); tstop=std::stod(bufStop);}catch(...){dt=0;tstop=0;}
                        if (dt<=0 || tstop<=0) { tranErr="Enter positive dt and tstop"; }
                        else {
                            last_dt = dt; last_tstop = tstop;
                            closeTransientDialog();
                            runTransientAndPrint(dt,tstop);
                        }
                    }
                }
                continue;
            }

            // --- Modal: AC Sweep (new) ---
            if (showACDialog) {
                if (e.type==SDL_MOUSEBUTTONDOWN && e.button.button==SDL_BUTTON_LEFT) {
                    SDL_Rect dlg{ W/2-300, H/2-170, 600, 340 };
                    SDL_Rect txtF1{ dlg.x+140, dlg.y+56,  140, 36 };
                    SDL_Rect txtF2{ dlg.x+428, dlg.y+56,  140, 36 };
                    SDL_Rect txtN { dlg.x+140, dlg.y+108, 140, 36 };
                    SDL_Rect chkLog{ dlg.x+428, dlg.y+114, 18, 18 };
                    SDL_Rect txtPPP{ dlg.x+140, dlg.y+160, 140, 36 };
                    SDL_Rect txtSet{ dlg.x+140, dlg.y+212, 140, 36 };
                    SDL_Rect txtMea{ dlg.x+428, dlg.y+212, 140, 36 };
                    SDL_Rect btnOk{ dlg.x+dlg.w-200, dlg.y+dlg.h-56, 84, 32 };
                    SDL_Rect btnCancel{ dlg.x+dlg.w-104, dlg.y+dlg.h-56, 84, 32 };

                    auto in=[&](SDL_Rect a){return mx>=a.x && mx<=a.x+a.w && my>=a.y && my<=a.y+a.h;};

                    if (in(txtF1)) acFocus=0; else
                    if (in(txtF2)) acFocus=1; else
                    if (in(txtN )) acFocus=2; else
                    if (in(txtPPP)) acFocus=3; else
                    if (in(txtSet)) acFocus=4; else
                    if (in(txtMea)) acFocus=5; else
                    if (in(chkLog)) { acLogspace = !acLogspace; } else
                    if (in(btnOk)) {
                        double f1=0,f2=0; int npts=0, ppp=0, settle=0, meas=0;
                        try{ f1=std::stod(bufF1); }catch(...){ f1=0; }
                        try{ f2=std::stod(bufF2); }catch(...){ f2=0; }
                        try{ npts=std::stoi(bufNpts); }catch(...){ npts=0; }
                        try{ ppp =std::stoi(bufPPP); }catch(...){ ppp=0; }
                        try{ settle=std::stoi(bufSettle); }catch(...){ settle=0; }
                        try{ meas=std::stoi(bufMeas); }catch(...){ meas=0; }

                        if (f1<=0 || f2<f1 || npts<2 || ppp<8 || meas<1) {
                            acErr = "Check: fstart>0, fstop>=fstart, npts>=2, pts/period>=8, measure>=1";
                        } else {
                            last_fstart=f1; last_fstop=f2; last_npts=npts; last_logspace=acLogspace;
                            last_ppp=ppp; last_settle=settle; last_measure=meas;
                            closeACDialog();
                            runACSweepAndPlot(f1,f2,npts,acLogspace,ppp,settle,meas);
                        }
                    } else
                    if (in(btnCancel)) { closeACDialog(); }
                }
                if (e.type==SDL_TEXTINPUT) {
                    char* buf = nullptr;
                    switch (acFocus) {
                        case 0: buf=bufF1; break; case 1: buf=bufF2; break; case 2: buf=bufNpts; break;
                        case 3: buf=bufPPP; break; case 4: buf=bufSettle; break; case 5: buf=bufMeas; break;
                    }
                    if (buf) {
                        size_t L = strlen(buf);
                        if (L < 60) { strcat(buf, e.text.text); }
                    }
                }
                if (e.type==SDL_KEYDOWN) {
                    if (e.key.keysym.sym==SDLK_BACKSPACE) {
                        char* buf = nullptr;
                        switch (acFocus) {
                            case 0: buf=bufF1; break; case 1: buf=bufF2; break; case 2: buf=bufNpts; break;
                            case 3: buf=bufPPP; break; case 4: buf=bufSettle; break; case 5: buf=bufMeas; break;
                        }
                        if (buf) {
                            size_t L = strlen(buf);
                            if (L>0) buf[L-1]=0;
                        }
                    }
                    if (e.key.keysym.sym==SDLK_TAB) acFocus = (acFocus+1)%6;
                    if (e.key.keysym.sym==SDLK_RETURN) {
                        double f1=0,f2=0; int npts=0, ppp=0, settle=0, meas=0;
                        try{ f1=std::stod(bufF1); }catch(...){ f1=0; }
                        try{ f2=std::stod(bufF2); }catch(...){ f2=0; }
                        try{ npts=std::stoi(bufNpts); }catch(...){ npts=0; }
                        try{ ppp =std::stoi(bufPPP); }catch(...){ ppp=0; }
                        try{ settle=std::stoi(bufSettle); }catch(...){ settle=0; }
                        try{ meas=std::stoi(bufMeas); }catch(...){ meas=0; }

                        if (f1<=0 || f2<f1 || npts<2 || ppp<8 || meas<1) {
                            acErr = "Check: fstart>0, fstop>=fstart, npts>=2, pts/period>=8, measure>=1";
                        } else {
                            last_fstart=f1; last_fstop=f2; last_npts=npts; last_logspace=acLogspace;
                            last_ppp=ppp; last_settle=settle; last_measure=meas;
                            closeACDialog();
                            runACSweepAndPlot(f1,f2,npts,acLogspace,ppp,settle,meas);
                        }
                    }
                }
                continue;
            }

            // --- Return menu (modal) ---
            if (showReturnMenu) {
                if (e.type==SDL_MOUSEBUTTONDOWN && e.button.button==SDL_BUTTON_LEFT) {
                    SDL_Rect dlg{ W/2-300, H/2-130, 600, 260 };
                    SDL_Rect txtName{ dlg.x+24, dlg.y+56,  400, 36 };
                    SDL_Rect btnReturn2{ dlg.x+24, dlg.y+dlg.h-56, 140, 36 };
                    SDL_Rect btnSave{ dlg.x+dlg.w-320, dlg.y+dlg.h-56, 140, 36 };
                    SDL_Rect btnDont{ dlg.x+dlg.w-160, dlg.y+dlg.h-56, 140, 36 };

                    auto in=[&](SDL_Rect a){ return mx>=a.x && mx<=a.x+a.w && my>=a.y && my<=a.y+a.h; };

                    if (in(txtName)) {
                        // focus stays on projectNameBuf
                    }
                    else if (in(btnReturn2)) {
                        showReturnMenu = false;
                        SDL_StopTextInput();
                    }
                    else if (in(btnSave)) {
                        std::string name = projectNameBuf;
                        if (name.empty()) name = "project";
                        for (char& ch : name) { if (ch=='/'||ch=='\\') ch='_'; }
                        gProjectPath = "D:\\OOP\\shemas\\" + name + ".txt";
                        save_circuit_to_file(gProjectPath, gCircuit);
                        ResetModelsAndUI(); ResetCounters(); blocks.clear();
                        SDL_StopTextInput();
                        running = false;
                    }
                    else if (in(btnDont)) {
                        ResetModelsAndUI(); ResetCounters(); blocks.clear();
                        SDL_StopTextInput();
                        running=false;
                    }
                }
                if (e.type==SDL_TEXTINPUT) {
                    size_t L = strlen(projectNameBuf);
                    if (L < sizeof(projectNameBuf)-1) { strcat(projectNameBuf, e.text.text); }
                }
                if (e.type==SDL_KEYDOWN) {
                    if (e.key.keysym.sym==SDLK_BACKSPACE) {
                        size_t L = strlen(projectNameBuf);
                        if (L>0) projectNameBuf[L-1]=0;
                    }
                    if (e.key.keysym.sym==SDLK_RETURN) {
                        std::string name = projectNameBuf;
                        if (name.empty()) name = "project";
                        for (char& ch : name) { if (ch=='/'||ch=='\\') ch='_'; }
                        gProjectPath = "D:\\OOP\\shemas\\" + name + ".txt";
                        save_circuit_to_file(gProjectPath, gCircuit);
                        ResetModelsAndUI(); ResetCounters(); blocks.clear();
                        SDL_StopTextInput();
                        running=false;
                    }
                }
                continue;
            }

            // --- Normal editor events ---
            if (e.type==SDL_MOUSEBUTTONDOWN && e.button.button==SDL_BUTTON_LEFT) {

                // top bar
                if (inside(btnReturn,mx,my)) {
                    buildProjectNameBufFromPath();
                    showReturnMenu = true;
                    SDL_StartTextInput();      // enable typing in project name
                    continue;
                }

                // Probes
                if (analysis_complete) {
                    if (inside(btnVProbe,mx,my)) { enterProbe(ProbeMode::V); continue; }
                    if (inside(btnIProbe,mx,my)) { enterProbe(ProbeMode::I); continue; }
                    if (inside(btnPProbe,mx,my)) { enterProbe(ProbeMode::P); continue; }
                }

                if (inside(btnGND,mx,my))  { groundPickMode=!groundPickMode; pending.active=false; probeMode=ProbeMode::None; continue; }
                if (inside(btnTran,mx,my)) { pending.active=false; groundPickMode=false; probeMode=ProbeMode::None; startTransientDialog(); continue; }
                if (inside(btnAC,mx,my))   { pending.active=false; groundPickMode=false; probeMode=ProbeMode::None; startACDialog(); continue; } // NEW
                if (inside(btnPhase,mx,my)){ /* later */ continue; }

                // If a probe is active, handle & consume
                if (analysis_complete && probeMode != ProbeMode::None) {
                    bool consumed=false;
                    if (probeMode == ProbeMode::V) {
                        for (int bi=(int)blocks.size()-1; bi>=0 && !consumed; --bi) {
                            auto& b = blocks[bi];
                            std::array<SDL_Rect,4> prs; int cnt=0; pinRects(b, prs, cnt);
                            for (int pi=0; pi<cnt; ++pi) if (inside(prs[pi],mx,my)) {
                                    if (Node** P = nodePtrForPin(b, pi)) { plotNodeVoltage(*P); consumed=true; }
                                    pending.active=false; break;
                                }
                        }
                    } else {
                        for (int i=(int)blocks.size()-1; i>=0 && !consumed; --i) if (inside(blocks[i].rect,mx,my)) {
                                if (probeMode==ProbeMode::I)  plotElementCurrent(blocks[i].elem);
                                if (probeMode==ProbeMode::P)  plotElementPower(blocks[i].elem);
                                pending.active=false; consumed=true;
                            }
                    }
                    continue;
                }

                // palette pick
                bool hitPalette=false;
                for (auto& it: palRects) if (inside(it.rect,mx,my)) { brush=it.label; hitPalette=true; break; }
                if (hitPalette) continue;

                // pin clicking (wiring, ground picking, node double-click)
                {
                    bool clickedPin=false;
                    for (int bi=(int)blocks.size()-1; bi>=0 && !clickedPin; --bi) {
                        auto& b = blocks[bi];
                        std::array<SDL_Rect,4> prs; int cnt=0; pinRects(b, prs, cnt);
                        for (int pi=0; pi<cnt; ++pi) if (inside(prs[pi], mx, my)) {
                                clickedPin=true;
                                SDL_Point pc = pinCenter(prs[pi]);

                                Uint32 now = SDL_GetTicks();
                                // Double-click node -> rename per-pin
                                if (lastPinBlock == bi && lastPinIndex == pi && (now - lastPinClickTicks) < 350) {
                                    lastPinBlock=-1; lastPinIndex=-1; lastPinClickTicks=0;
                                    if (Node** P = nodePtrForPin(b, pi)) startNodeDialog(P);
                                    break;
                                } else {
                                    lastPinBlock = bi; lastPinIndex = pi; lastPinClickTicks = now;
                                }

                                if (groundPickMode) {
                                    if (Node** P = nodePtrForPin(b, pi)) if (*P) {
                                            for (auto* n : gCircuit.nodes) if (n) n->IsG=false;
                                            (*P)->IsG = true;
                                            groundPickMode=false;
                                            pending.active=false;
                                            if (!gProjectPath.empty()) save_circuit_to_file(gProjectPath, gCircuit);
                                        }
                                    break;
                                }

                                // wiring (rubber-band)
                                if (!pending.active) {
                                    pending.active = true;
                                    pending.comp = b.elem;
                                    pending.pin  = pi;
                                    pending.p    = pc;
                                } else {
                                    if (!(pending.comp == b.elem && pending.pin == pi)) {
                                        Node** A = nodePtrForPin(b, pi);
                                        Node** B = nullptr;
                                        for (auto& bb : blocks) if (bb.elem == pending.comp) { B = nodePtrForPin(bb, pending.pin); break; }
                                        if (A && B && *A && *B) {
                                            *A = *B; // connect to same node (merge)
                                            rebuildWiresFromNodes();
                                            if (!gProjectPath.empty()) save_circuit_to_file(gProjectPath, gCircuit);
                                        }
                                    }
                                    pending.active = false;
                                }
                                break;
                            }
                    }
                    if (clickedPin) continue;
                }

                // block double-click -> open combined element dialog
                for (int i=(int)blocks.size()-1; i>=0; --i) if (inside(blocks[i].rect,mx,my)) {
                        Uint32 now = SDL_GetTicks();
                        if (lastClickBlock == i && (now - lastClickTicks) < 350) {
                            openElementDialog(blocks[i].elem, i);
                            lastClickBlock = -1; lastClickTicks = 0;
                            break;
                        } else {
                            dragging=true; dragIdx=i; dragDX=mx-blocks[i].rect.x; dragDY=my-blocks[i].rect.y;
                            lastClickBlock = i; lastClickTicks = now;
                            break;
                        }
                    }
                if (dragging) continue;

                // place from palette
                if (!brush.empty() && my < H - PALETTE_H) {
                    SDL_Rect rc{ mx - 70, my - 22, ITEM_W, ITEM_H };
                    int cx = rc.x, cy = rc.y; clamp(cx, cy, rc); rc.x=cx; rc.y=cy;

                    std::string elemName;
                    if (AddElementForType(gCircuit, brush, &elemName)) {
                        ui_set_element_position(elemName, rc.x, rc.y, rc.w, rc.h);
                        blocks.push_back(Block{rc, brush, elemName});
                        rebuildWiresFromNodes();
                        if (!gProjectPath.empty()) save_circuit_to_file(gProjectPath, gCircuit);
                    }
                    brush.clear();
                }
            }

            if (e.type == SDL_MOUSEMOTION) {
                if (probeMode != ProbeMode::None) {
                    dragging = false;
                } else if (dragging && dragIdx >= 0) {
                    int nx = mx - dragDX, ny = my - dragDY;
                    SDL_Rect rc = blocks[dragIdx].rect; rc.x=nx; rc.y=ny;
                    clamp(nx, ny, rc);
                    blocks[dragIdx].rect.x = nx;
                    blocks[dragIdx].rect.y = ny;
                    ui_set_element_position(blocks[dragIdx].elem, nx, ny, blocks[dragIdx].rect.w, blocks[dragIdx].rect.h);
                }
            }

            if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT) {
                if (dragIdx >= 0) {
                    if (!gProjectPath.empty()) save_circuit_to_file(gProjectPath, gCircuit);
                }
                dragging=false; dragIdx=-1;
            }
        }
        if (!running) break;

        // draw editor UI
        SDL_SetRenderDrawColor(r,14,18,28,255); SDL_RenderClear(r);
        boxRGBA(r,0,0,W,60,20,26,40,255);

        std::string hint = (probeMode==ProbeMode::V) ? " | Voltage probe: click a node"
                                                     : (probeMode==ProbeMode::I) ? " | Current probe: click an element"
                                                                                 : (probeMode==ProbeMode::P) ? " | Power probe: click an element"
                                                                                                             : "";
        drawText(r, fontTitle,
                 (groundPickMode? "Ground-pick: click a node square to set GND (exits after pick)"
                                : ("Schematic Editor — place, drag, double-click to edit. "
                                   "Click node squares to wire / double-click to rename node." + hint)),
                 20, 18, SDL_Color{255,255,255,255});

        // Probe buttons
        if (analysis_complete) {
            drawBtn(btnVProbe, "V Probe", probeMode==ProbeMode::V);
            drawBtn(btnIProbe, "I Probe", probeMode==ProbeMode::I);
            drawBtn(btnPProbe, "P Probe", probeMode==ProbeMode::P);
        }

        drawBtn(btnGND,"GND",groundPickMode);
        drawBtn(btnTran,"Transient",false);
        drawBtn(btnAC,"AC Sweep",false);
        drawBtn(btnPhase,"Phase",false);

        bool hotRet = ([](const SDL_Rect& rc){
            int mx,my; SDL_GetMouseState(&mx,&my);
            return mx>=rc.x && mx<=rc.x+rc.w && my>=rc.y && my<=rc.y+rc.h;
        })(btnReturn);
        roundedBoxRGBA(r, btnReturn.x, btnReturn.y, btnReturn.x+btnReturn.w, btnReturn.y+btnReturn.h, 8,
                       hotRet?180:140, 40, 50, hotRet?255:220);
        roundedRectangleRGBA(r, btnReturn.x, btnReturn.y, btnReturn.x+btnReturn.w, btnReturn.y+btnReturn.h, 8,
                             60,20,30,255);
        drawText(r, fontBtn, "Return", btnReturn.x+14, btnReturn.y+7, SDL_Color{255,255,255,255});

        roundedRectangleRGBA(r, 12, 70, W-12, H-PALETTE_H-12, 12, 90,120,180,255);

        // wires
        for (const auto& wv : wires) {
            const Block *A=nullptr, *B=nullptr;
            for (auto& b : blocks) { if (b.elem==wv.aComp) A=&b; if (b.elem==wv.bComp) B=&b; }
            if (!A || !B) continue;
            std::array<SDL_Rect,4> pa, pb; int ca=0, cb=0; pinRects(*A,pa,ca); pinRects(*B,pb,cb);
            if (wv.aPin>=ca || wv.bPin>=cb) continue;
            SDL_Point p1 = pinCenter(pa[wv.aPin]);
            SDL_Point p2 = pinCenter(pb[wv.bPin]);
            thickLineRGBA(r, p1.x, p1.y, p2.x, p2.y, 2, 200,220,255,255);
        }

        // blocks + pins (ALL NODES same color; GND = green)
        for (auto& b: blocks) {
            Uint8 R,G,B; colorForBlock(b,R,G,B);
            roundedBoxRGBA(r, b.rect.x, b.rect.y, b.rect.x+b.rect.w, b.rect.y+b.rect.h, 10, R,G,B,220);
            roundedRectangleRGBA(r, b.rect.x, b.rect.y, b.rect.x+b.rect.w, b.rect.y+b.rect.h, 10, 40,70,120,255);
            drawText(r, fontBtn, b.type, b.rect.x+10, b.rect.y + b.rect.h/2 - 10, SDL_Color{20,20,30,255});

            // pin squares
            std::array<SDL_Rect,4> prs; int cnt=0; pinRects(b,prs,cnt);
            for (int i=0;i<cnt;++i) {
                Node** P = nodePtrForPin(b, i);
                bool isG = (P && *P && (*P)->IsG);
                Uint8 rr = isG ? 80 : 40, gg = isG ? 220 : 200, bb = isG ? 100 : 220;
                boxRGBA(r, prs[i].x, prs[i].y, prs[i].x+prs[i].w, prs[i].y+prs[i].h, rr,gg,bb,255);
                rectangleRGBA(r, prs[i].x, prs[i].y, prs[i].x+prs[i].w, prs[i].y+prs[i].h, 20,40,60,255);
            }
        }

        // rubber-band
        if (pending.active && probeMode==ProbeMode::None) {
            int mx2,my2; SDL_GetMouseState(&mx2,&my2);
            thickLineRGBA(r, pending.p.x, pending.p.y, mx2, my2, 2, 200,220,255,200);
        }

        // palette
        boxRGBA(r, 0, H-PALETTE_H, W, H, 20,26,40,255);
        drawText(r, fontTitle, "Palette", 16, H-PALETTE_H+6, SDL_Color{255,255,255,255});
        for (auto& it: palRects) {
            int mmx,mmy; SDL_GetMouseState(&mmx,&mmy);
            bool hi = inside(it.rect, mmx, mmy);
            Uint8 a = hi?255:210; Uint8 pR,pG,pB; colorForPalette(it.label,pR,pG,pB);
            roundedBoxRGBA(r, it.rect.x, it.rect.y, it.rect.x+it.rect.w, it.rect.y+it.rect.h, ITEM_RADIUS, pR,pG,pB,a);
            roundedRectangleRGBA(r, it.rect.x, it.rect.y, it.rect.x+it.rect.w, it.rect.y+it.rect.h, ITEM_RADIUS, 40,70,120,255);
            drawText(r, fontBtn, it.label, it.rect.x+10, it.rect.y + it.rect.h/2 - 10, SDL_Color{20,20,30,255});
        }

        // --- Draw modals ---
        if (showTranDialog) {
            boxRGBA(r, 0,0,W,H, 0,0,0,120);
            SDL_Rect dlg{ W/2-260, H/2-120, 520, 240 };
            roundedBoxRGBA(r, dlg.x, dlg.y, dlg.x+dlg.w, dlg.y+dlg.h, 10, 250,252,255,255);
            roundedRectangleRGBA(r, dlg.x, dlg.y, dlg.x+dlg.w, dlg.y+dlg.h, 10, 60,80,120,255);
            drawTextRGBA(fontTitle, "Transient Settings", dlg.x+20, dlg.y+16, 40,60,90);

            SDL_Rect lblDt{ dlg.x+24, dlg.y+60, 90, 28 };
            SDL_Rect txtDt{ dlg.x+120, dlg.y+56,  160, 36 };
            SDL_Rect lblSt{ dlg.x+300, dlg.y+60, 90, 28 };
            SDL_Rect txtSt{ dlg.x+396, dlg.y+56,  100, 36 };
            SDL_Rect btnOk{ dlg.x+dlg.w-200, dlg.y+dlg.h-56, 84, 32 };
            SDL_Rect btnCancel{ dlg.x+dlg.w-104, dlg.y+dlg.h-56, 84, 32 };

            drawTextRGBA(fontBtn, "dt (s)", lblDt.x, lblDt.y, 40,60,90);
            drawTextFieldCard(txtDt, bufDt, tranFocus==0);
            drawTextRGBA(fontBtn, "tstop (s)", lblSt.x, lblSt.y, 40,60,90);
            drawTextFieldCard(txtSt, bufStop, tranFocus==1);

            auto drawDlgBtn = [&](const SDL_Rect& rc, const char* lab){
                int mx,my; SDL_GetMouseState(&mx,&my);
                bool h = inside(rc, mx, my);
                Uint8 fr = h? 70: 50, fg = h? 160:130, fb = h? 240:200;
                roundedBoxRGBA(r, rc.x, rc.y, rc.x+rc.w, rc.y+rc.h, 8, fr,fg,fb,255);
                roundedRectangleRGBA(r, rc.x, rc.y, rc.x+rc.w, rc.y+rc.h, 8, 50,70,110,255);
                drawText(r, fontBtn, lab, rc.x+16, rc.y+7, SDL_Color{255,255,255,255});
            };
            drawDlgBtn(btnOk, "OK");
            drawDlgBtn(btnCancel, "Cancel");

            if (!tranErr.empty()) drawTextRGBA(fontBtn, tranErr, dlg.x+20, dlg.y+dlg.h-92, 180,40,40);
        }

        // AC Sweep dialog (new)
        if (showACDialog) {
            boxRGBA(r, 0,0,W,H, 0,0,0,120);
            SDL_Rect dlg{ W/2-300, H/2-170, 600, 340 };
            roundedBoxRGBA(r, dlg.x, dlg.y, dlg.x+dlg.w, dlg.y+dlg.h, 10, 250,252,255,255);
            roundedRectangleRGBA(r, dlg.x, dlg.y, dlg.x+dlg.w, dlg.y+dlg.h, 10, 60,80,120,255);
            drawTextRGBA(fontTitle, "AC Sweep Settings", dlg.x+20, dlg.y+16, 40,60,90);

            SDL_Rect lblF1{ dlg.x+24, dlg.y+60,  110, 28 };
            SDL_Rect txtF1{ dlg.x+140, dlg.y+56, 140, 36 };
            SDL_Rect lblF2{ dlg.x+320, dlg.y+60,  100, 28 };
            SDL_Rect txtF2{ dlg.x+428, dlg.y+56, 140, 36 };

            SDL_Rect lblN { dlg.x+24, dlg.y+112, 110, 28 };
            SDL_Rect txtN { dlg.x+140, dlg.y+108, 140, 36 };
            SDL_Rect lblLog{ dlg.x+320, dlg.y+112, 120, 28 };
            SDL_Rect chkLog{ dlg.x+428, dlg.y+114, 18, 18 };

            SDL_Rect lblPPP{ dlg.x+24, dlg.y+164, 110, 28 };
            SDL_Rect txtPPP{ dlg.x+140, dlg.y+160, 140, 36 };

            SDL_Rect lblSet{ dlg.x+24, dlg.y+216, 110, 28 };
            SDL_Rect txtSet{ dlg.x+140, dlg.y+212, 140, 36 };

            SDL_Rect lblMea{ dlg.x+320, dlg.y+216, 120, 28 };
            SDL_Rect txtMea{ dlg.x+428, dlg.y+212, 140, 36 };

            SDL_Rect btnOk{ dlg.x+dlg.w-200, dlg.y+dlg.h-56, 84, 32 };
            SDL_Rect btnCancel{ dlg.x+dlg.w-104, dlg.y+dlg.h-56, 84, 32 };

            drawTextRGBA(fontBtn, "f start (Hz)", lblF1.x, lblF1.y, 40,60,90);
            drawTextFieldCard(txtF1, bufF1, acFocus==0);
            drawTextRGBA(fontBtn, "f stop (Hz)",  lblF2.x, lblF2.y, 40,60,90);
            drawTextFieldCard(txtF2, bufF2, acFocus==1);

            drawTextRGBA(fontBtn, "points",       lblN.x,  lblN.y,  40,60,90);
            drawTextFieldCard(txtN,  bufNpts, acFocus==2);

            drawTextRGBA(fontBtn, "log spacing",  lblLog.x, lblLog.y, 40,60,90);
            roundedRectangleRGBA(r, chkLog.x, chkLog.y, chkLog.x+chkLog.w, chkLog.y+chkLog.h, 3, 60,80,120,255);
            if (acLogspace)
                boxRGBA(r, chkLog.x+3, chkLog.y+3, chkLog.x+chkLog.w-3, chkLog.y+chkLog.h-3, 80,220,100,255);

            drawTextRGBA(fontBtn, "pts / period", lblPPP.x, lblPPP.y, 40,60,90);
            drawTextFieldCard(txtPPP, bufPPP, acFocus==3);

            drawTextRGBA(fontBtn, "settle cycles", lblSet.x, lblSet.y, 40,60,90);
            drawTextFieldCard(txtSet, bufSettle, acFocus==4);

            drawTextRGBA(fontBtn, "measure cycles", lblMea.x, lblMea.y, 40,60,90);
            drawTextFieldCard(txtMea, bufMeas, acFocus==5);

            auto drawDlgBtn = [&](const SDL_Rect& rc, const char* lab){
                int mx,my; SDL_GetMouseState(&mx,&my);
                bool h = inside(rc, mx, my);
                Uint8 fr = h? 70: 50, fg = h? 160:130, fb = h? 240:200;
                roundedBoxRGBA(r, rc.x, rc.y, rc.x+rc.w, rc.y+rc.h, 8, fr,fg,fb,255);
                roundedRectangleRGBA(r, rc.x, rc.y, rc.x+rc.w, rc.y+rc.h, 8, 50,70,110,255);
                drawText(r, fontBtn, lab, rc.x+16, rc.y+7, SDL_Color{255,255,255,255});
            };
            drawDlgBtn(btnOk, "OK");
            drawDlgBtn(btnCancel, "Cancel");

            if (!acErr.empty()) drawTextRGBA(fontBtn, acErr, dlg.x+20, dlg.y+dlg.h-92, 180,40,40);
        }

        if (showNodeDialog) {
            boxRGBA(r, 0,0,W,H, 0,0,0,120);
            SDL_Rect dlg{ W/2-240, H/2-110, 480, 220 };
            roundedBoxRGBA(r, dlg.x, dlg.y, dlg.x+dlg.w, dlg.y+dlg.h, 10, 250,252,255,255);
            roundedRectangleRGBA(r, dlg.x, dlg.y, dlg.x+dlg.w, dlg.y+dlg.h, 10, 60,80,120,255);
            drawTextRGBA(fontTitle, "Edit Node", dlg.x+20, dlg.y+16, 40,60,90);

            SDL_Rect lblName{ dlg.x+24, dlg.y+60, 90, 28 };
            SDL_Rect txtName{ dlg.x+24, dlg.y+56,  280, 36 };
            SDL_Rect chkGnd { dlg.x+24, dlg.y+104,  18, 18 };
            SDL_Rect lblGnd { dlg.x+48, dlg.y+100,  200, 28 };
            SDL_Rect btnOk{ dlg.x+dlg.w-200, dlg.y+dlg.h-56, 84, 32 };
            SDL_Rect btnCancel{ dlg.x+dlg.w-104, dlg.y+dlg.h-56, 84, 32 };

            drawTextRGBA(fontBtn, "Name", lblName.x, lblName.y, 40,60,90);
            drawTextFieldCard(txtName, nodeDlg.buf, true);

            roundedRectangleRGBA(r, chkGnd.x, chkGnd.y, chkGnd.x+chkGnd.w, chkGnd.y+chkGnd.h, 3, 60,80,120,255);
            if (nodeDlg.wantGround)
                boxRGBA(r, chkGnd.x+3, chkGnd.y+3, chkGnd.x+chkGnd.w-3, chkGnd.y+chkGnd.h-3, 80,220,100,255);
            drawTextRGBA(fontBtn, "Ground this node", lblGnd.x, lblGnd.y, 40,60,90);

            auto drawDlgBtn = [&](const SDL_Rect& rc, const char* lab){
                int mx,my; SDL_GetMouseState(&mx,&my);
                bool h = inside(rc, mx, my);
                Uint8 fr = h? 70: 50, fg = h? 160:130, fb = h? 240:200;
                roundedBoxRGBA(r, rc.x, rc.y, rc.x+rc.w, rc.y+rc.h, 8, fr,fg,fb,255);
                roundedRectangleRGBA(r, rc.x, rc.y, rc.x+rc.w, rc.y+rc.h, 8, 50,70,110,255);
                drawText(r, fontBtn, lab, rc.x+16, rc.y+7, SDL_Color{255,255,255,255});
            };
            drawDlgBtn(btnOk, "OK");
            drawDlgBtn(btnCancel, "Cancel");
        }

        if (showElemDialog) {
            boxRGBA(r, 0,0,W,H, 0,0,0,120);
            SDL_Rect dlg{ W/2-300, H/2-140, 600, 280 };
            roundedBoxRGBA(r, dlg.x, dlg.y, dlg.x+dlg.w, dlg.y+dlg.h, 10, 250,252,255,255);
            roundedRectangleRGBA(r, dlg.x, dlg.y, dlg.x+dlg.w, dlg.y+dlg.h, 10, 60,80,120,255);
            drawTextRGBA(fontTitle, "Edit Element", dlg.x+20, dlg.y+16, 40,60,90);

            SDL_Rect lblName{ dlg.x+24, dlg.y+30, 200, 24 };
            SDL_Rect txtName{ dlg.x+24, dlg.y+58,  360, 36 };
            SDL_Rect btnParam{ dlg.x+24, dlg.y+110, 140, 32 };
            SDL_Rect btnDelete{ dlg.x+dlg.w-320, dlg.y+dlg.h-100, 276, 36 };
            SDL_Rect btnSave{ dlg.x+dlg.w-320, dlg.y+dlg.h-56, 140, 36 };
            SDL_Rect btnBack{ dlg.x+dlg.w-160, dlg.y+dlg.h-56, 140, 36 };

            drawTextRGBA(fontBtn, "Name", lblName.x, lblName.y, 40,60,90);
            drawTextFieldCard(txtName, elemDlg.nameBuf, true);

            // Parameters button
            auto drawDlgBtn = [&](const SDL_Rect& rc, const char* lab, bool warn=false){
                int mx,my; SDL_GetMouseState(&mx,&my);
                bool h = inside(rc, mx, my);
                Uint8 fr = warn ? (h? 180:160) : (h? 70:50);
                Uint8 fg = warn ? (h? 60:40)   : (h?160:130);
                Uint8 fb = warn ? (h? 70:50)   : (h?240:200);
                roundedBoxRGBA(r, rc.x, rc.y, rc.x+rc.w, rc.y+rc.h, 8, fr,fg,fb,255);
                roundedRectangleRGBA(r, rc.x, rc.y, rc.x+rc.w, rc.y+rc.h, 8, 50,70,110,255);
                drawText(r, fontBtn, lab, rc.x+16, rc.y+7, SDL_Color{255,255,255,255});
            };
            drawDlgBtn(btnParam, "Parameters");
            drawDlgBtn(btnDelete, "Delete Element", /*warn=*/true);
            drawDlgBtn(btnSave, "Save");
            drawDlgBtn(btnBack, "Back");

            if (!elemDlg.err.empty()) {
                drawTextRGBA(fontBtn, elemDlg.err, dlg.x+24, dlg.y+dlg.h-132, 180,40,40);
            }
        }

        if (showReturnMenu) {
            boxRGBA(r, 0,0,W,H, 0,0,0,120);
            SDL_Rect dlg{ W/2-300, H/2-130, 600, 260 };
            roundedBoxRGBA(r, dlg.x, dlg.y, dlg.x+dlg.w, dlg.y+dlg.h, 10, 250,252,255,255);
            roundedRectangleRGBA(r, dlg.x, dlg.y, dlg.x+dlg.w, dlg.y+dlg.h, 10, 60,80,120,255);
            drawTextRGBA(fontTitle, "Project options", dlg.x+20, dlg.y+16, 40,60,90);

            SDL_Rect lblName{ dlg.x+24, dlg.y+60, 180, 28 };
            SDL_Rect txtName{ dlg.x+24, dlg.y+56,  400, 36 };
            SDL_Rect btnReturn2{ dlg.x+24, dlg.y+dlg.h-56, 140, 36 };
            SDL_Rect btnSave{ dlg.x+dlg.w-320, dlg.y+dlg.h-56, 140, 36 };
            SDL_Rect btnDont{ dlg.x+dlg.w-160, dlg.y+dlg.h-56, 140, 36 };

            drawTextRGBA(fontBtn, "Project file name", lblName.x, lblName.y, 40,60,90);
            drawTextFieldCard(txtName, projectNameBuf, true);

            auto drawDlgBtn = [&](const SDL_Rect& rc, const char* lab){
                int mx,my; SDL_GetMouseState(&mx,&my);
                bool h = inside(rc, mx, my);
                Uint8 fr = h? 70: 50, fg = h? 160:130, fb = h? 240:200;
                roundedBoxRGBA(r, rc.x, rc.y, rc.x+rc.w, rc.y+rc.h, 8, fr,fg,fb,255);
                roundedRectangleRGBA(r, rc.x, rc.y, rc.x+rc.w, rc.y+rc.h, 8, 50,70,110,255);
                drawText(r, fontBtn, lab, rc.x+16, rc.y+7, SDL_Color{255,255,255,255});
            };
            drawDlgBtn(btnReturn2, "Return");
            drawDlgBtn(btnSave, "Save");
            drawDlgBtn(btnDont, "Don't Save");
        }

        SDL_RenderPresent(r);

        // Render results window
        if (resultsWin && resultsWin->isOpen()) resultsWin->renderFrame();
        else if (resultsWin && resultsWin->isClosed()) { delete resultsWin; resultsWin = nullptr; }
    }

    if (resultsWin) { delete resultsWin; resultsWin = nullptr; }
    if (fontBtn)   TTF_CloseFont(fontBtn);
    if (fontTitle) TTF_CloseFont(fontTitle);
    SDL_DestroyRenderer(r);
    SDL_DestroyWindow(win);
}





void ShowOpenProjectWindow() {
    const int W = 900, H = 600;
    SDL_Window* win = SDL_CreateWindow(
            "Open Project",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, W, H, SDL_WINDOW_SHOWN);
    if (!win) return;

    SDL_Renderer* r = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!r) { SDL_DestroyWindow(win); return; }

    TTF_Font* fontTitle = loadUIFont(20);
    TTF_Font* fontBtn   = loadUIFont(18);

    extern std::string gProjectPath;
    const std::string projectsDir = "D:\\OOP\\shemas";

    struct Item { std::string name; std::string full; };
    std::vector<Item> items;

    auto inside = [](const SDL_Rect& a, int x, int y){
        return x>=a.x && x<=a.x+a.w && y>=a.y && y<=a.y+a.h;
    };

    // Enumerate *.txt projects (Win32)
    auto refreshList = [&](){
        items.clear();
        WIN32_FIND_DATAA fdata{};
        HANDLE hFind = FindFirstFileA((projectsDir + "\\*.txt").c_str(), &fdata);
        if (hFind != INVALID_HANDLE_VALUE) {
            do {
                if (!(fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                    std::string fn = fdata.cFileName;
                    items.push_back( Item{ fn, projectsDir + "\\" + fn } );
                }
            } while (FindNextFileA(hFind, &fdata));
            FindClose(hFind);
        }

        // Sort by filename (case-insensitive)
        std::sort(items.begin(), items.end(), [](const Item& a, const Item& b){
            std::string A=a.name, B=b.name;
            for (size_t i=0;i<A.size();++i) A[i] = (char)std::tolower((unsigned char)A[i]);
            for (size_t i=0;i<B.size();++i) B[i] = (char)std::tolower((unsigned char)B[i]);
            return A < B;
        });
    };
    refreshList();

    // Layout
    SDL_Rect btnBack{ W - 120, 12, 100, 36 };

    // List viewport (scrollable region)
    const int rowH = 44;
    const int pad  = 10;
    SDL_Rect listViewport{ 40, 90, W - 80, H - 130 };  // x, y, width, height
    const int scrollBarW = 10;
    const int scrollTrackPad = 2;

    // Scroll state
    float scroll = 0.0f;           // content scroll offset (pixels)
    bool  draggingScrollbar = false;
    int   dragStartY = 0;
    float scrollAtDragStart = 0.0f;

    auto contentHeight = [&]()->int {
        if (items.empty()) return 0;
        return int(items.size()) * (rowH + pad) - pad;
    };
    auto maxScroll = [&]()->float {
        int ch = contentHeight();
        return (ch > listViewport.h) ? float(ch - listViewport.h) : 0.0f;
    };
    auto clampScroll = [&](){
        float mx = maxScroll();
        if (scroll < 0.0f) scroll = 0.0f;
        if (scroll > mx)   scroll = mx;
    };

    // Rect for a given item index, accounting for scroll
    auto getItemRect = [&](size_t i)->SDL_Rect {
        int y = listViewport.y + int(i * (rowH + pad)) - int(scroll);
        // Leave room for scrollbar if needed
        int wAvail = listViewport.w - (maxScroll() > 0 ? (scrollBarW + 6) : 0);
        return SDL_Rect{ listViewport.x, y, wAvail, rowH };
    };

    // Scrollbar geometry (track + thumb)
    auto getScrollBarRects = [&]()->std::pair<SDL_Rect, SDL_Rect> {
        SDL_Rect track{ listViewport.x + listViewport.w - scrollBarW, listViewport.y, scrollBarW, listViewport.h };
        int ch = contentHeight();
        if (ch <= listViewport.h) {
            // No scrollbar needed
            return std::make_pair(track, SDL_Rect{ track.x, track.y, track.w, 0 });
        }
        // Thumb size proportional to viewport/content
        int thumbH = std::max(20, int( (double)listViewport.h * (double)listViewport.h / (double)ch ));
        int travel = listViewport.h - thumbH - 2*scrollTrackPad;
        if (travel < 1) travel = 1;
        double ratio = (double)scroll / (double)(ch - listViewport.h);
        int thumbY = track.y + scrollTrackPad + int( ratio * travel );
        SDL_Rect thumb{ track.x, thumbY, track.w, thumbH };
        return std::make_pair(track, thumb);
    };

    auto drawBtn = [&](const SDL_Rect& rc, const char* label){
        int mx,my; SDL_GetMouseState(&mx,&my);
        bool h = inside(rc, mx, my);
        Uint8 fr = h? 160:120, fg = h? 60:40, fb = h? 80:50;
        roundedBoxRGBA(r, rc.x, rc.y, rc.x+rc.w, rc.y+rc.h, 8, fr,fg,fb, 240);
        roundedRectangleRGBA(r, rc.x, rc.y, rc.x+rc.w, rc.y+rc.h, 8, 60,20,30,255);
        drawText(r, fontBtn, label, rc.x+20, rc.y+7, SDL_Color{255,255,255,255});
    };

    auto scrollByPixels = [&](float dy){
        scroll += dy;
        clampScroll();
    };

    bool running = true;
    while (running) {
        SDL_Event e; int mx=0,my=0; SDL_GetMouseState(&mx,&my);
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = false;
            }
            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_ESCAPE:   running = false; break;
                    case SDLK_DOWN:     scrollByPixels( rowH ); break;
                    case SDLK_UP:       scrollByPixels(-rowH ); break;
                    case SDLK_PAGEDOWN: scrollByPixels( listViewport.h * 0.9f ); break;
                    case SDLK_PAGEUP:   scrollByPixels(-listViewport.h * 0.9f ); break;
                    case SDLK_HOME:     scroll = 0.0f; break;
                    case SDLK_END:      scroll = maxScroll(); break;
                }
            }
            if (e.type == SDL_MOUSEWHEEL) {
                // SDL wheel: positive y = scroll up (move content down)
                const float SCROLL_SPEED = 48.0f; // pixels per wheel notch
                scrollByPixels( -e.wheel.y * SCROLL_SPEED );
            }
            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                if (inside(btnBack,mx,my)) {
                    running = false; // back to main menu
                    break;
                }
                // Scrollbar drag start?
                std::pair<SDL_Rect, SDL_Rect> pairRT = getScrollBarRects();
                SDL_Rect track = pairRT.first;
                SDL_Rect thumb = pairRT.second;
                if (maxScroll() > 0 && inside(thumb, mx, my)) {
                    draggingScrollbar = true;
                    dragStartY = my;
                    scrollAtDragStart = scroll;
                } else if (inside(listViewport, mx, my)) {
                    // Click in the list: hit-test items (visible region)
                    for (size_t i=0;i<items.size();++i) {
                        SDL_Rect rc = getItemRect(i);
                        // Only consider if intersects viewport (visible)
                        if (rc.y + rc.h < listViewport.y || rc.y > listViewport.y + listViewport.h) continue;
                        if (inside(rc, mx, my)) {
                            // Open this project
                            gProjectPath = items[i].full;
                            ResetModelsAndUI();
                            ResetCounters();
                            read_circuit_from_file(gProjectPath, gCircuit, /*clearFirst=*/true);
                            SDL_DestroyRenderer(r);
                            SDL_DestroyWindow(win);
                            ShowSchematicEditorWindow(); // jump into editor
                            return;
                        }
                    }
                }
            }
            if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT) {
                draggingScrollbar = false;
            }
            if (e.type == SDL_MOUSEMOTION && draggingScrollbar) {
                std::pair<SDL_Rect, SDL_Rect> pairRT = getScrollBarRects();
                SDL_Rect track = pairRT.first;
                SDL_Rect thumb = pairRT.second;
                int ch = contentHeight();
                if (ch > listViewport.h && thumb.h > 0) {
                    int travel = listViewport.h - thumb.h - 2*scrollTrackPad;
                    if (travel < 1) travel = 1;
                    int dy = my - dragStartY;
                    double pixelsPerThumb = double(ch - listViewport.h) / double(travel);
                    scroll = float(scrollAtDragStart + dy * pixelsPerThumb);
                    clampScroll();
                }
            }
        }

        // draw frame
        SDL_SetRenderDrawColor(r, 14, 18, 28, 255); SDL_RenderClear(r);
        boxRGBA(r,0,0,W,60,20,26,40,255);
        drawText(r, fontTitle, "Open Existing Project", 20, 18, SDL_Color{255,255,255,255});
        drawBtn(btnBack, "Back");

        // Draw list background
        roundedRectangleRGBA(r, listViewport.x-2, listViewport.y-2,
                             listViewport.x+listViewport.w+2, listViewport.y+listViewport.h+2,
                             8, 60,80,120,255);

        if (items.empty()) {
            drawText(r, fontBtn, "No .txt projects found in D:\\OOP\\shemas\\", listViewport.x, listViewport.y, SDL_Color{255,255,255,255});
        } else {
            // Draw each visible item (respect viewport)
            for (size_t i=0;i<items.size();++i) {
                SDL_Rect rc = getItemRect(i);
                if (rc.y + rc.h < listViewport.y || rc.y > listViewport.y + listViewport.h) continue;

                int mx2,my2; SDL_GetMouseState(&mx2,&my2);
                bool hi = inside(rc, mx2, my2);
                Uint8 a = hi ? 255 : 220;
                roundedBoxRGBA(r, rc.x, rc.y, rc.x+rc.w, rc.y+rc.h, 8, 50,80,120, a);
                roundedRectangleRGBA(r, rc.x, rc.y, rc.x+rc.w, rc.y+rc.h, 8, 30,50,90,255);
                drawText(r, fontBtn, items[i].name, rc.x + 16, rc.y + rc.h/2 - 10, SDL_Color{255,255,255,255});
            }

            // Draw scrollbar
            if (maxScroll() > 0) {
                std::pair<SDL_Rect, SDL_Rect> pairRT = getScrollBarRects();
                SDL_Rect track = pairRT.first;
                SDL_Rect thumb = pairRT.second;

                // Track
                roundedBoxRGBA(r, track.x, track.y, track.x+track.w, track.y+track.h, 4, 40,60,90, 180);
                // Thumb (highlight on hover/drag)
                int mx2,my2; SDL_GetMouseState(&mx2,&my2);
                bool over = inside(thumb, mx2, my2);
                Uint8 tr = draggingScrollbar ? 200 : (over ? 170 : 140);
                Uint8 tg = draggingScrollbar ? 200 : (over ? 170 : 140);
                Uint8 tb = draggingScrollbar ? 220 : (over ? 200 : 180);
                roundedBoxRGBA(r, thumb.x, thumb.y, thumb.x+thumb.w, thumb.y+thumb.h, 4, tr,tg,tb, 255);
                roundedRectangleRGBA(r, thumb.x, thumb.y, thumb.x+thumb.w, thumb.y+thumb.h, 4, 30,50,90,255);
            }
        }

        SDL_RenderPresent(r);
    }

    if (fontBtn)   TTF_CloseFont(fontBtn);
    if (fontTitle) TTF_CloseFont(fontTitle);
    SDL_DestroyRenderer(r);
    SDL_DestroyWindow(win);
}




// ----------------------------
// Main Menu (recreate-on-back)
// ----------------------------
void ShowMainMenuWindow() {
    if (SDL_Init(SDL_INIT_VIDEO)!=0) { std::cerr << "SDL_Init: " << SDL_GetError() << "\n"; return; }

    const int W=900, H=560;
    auto inside = [](const SDL_Rect& r, int x, int y){
        return x>=r.x && x<=r.x+r.w && y>=r.y && y<=r.y+r.h;
    };

    bool running = true;
    while (running) {
        SDL_Window* window = SDL_CreateWindow("Main Menu",
                                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, W, H, SDL_WINDOW_SHOWN);
        if (!window) break;
        SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (!renderer) { SDL_DestroyWindow(window); break; }

        TTF_Font* fontTitle = loadUIFont(20);
        TTF_Font* fontBtn   = loadUIFont(18);

        SDL_Rect panel   = { W/2 - 360, H/2 - 200, 720, 360 };
        SDL_Rect btnNew  = { panel.x + 160, panel.y +  90, 400, 60 };
        SDL_Rect btnShow = { panel.x + 160, panel.y + 170, 400, 60 };
        SDL_Rect btnExit = { panel.x + 160, panel.y + 250, 400, 60 };

        bool inMenu = true;
        while (inMenu) {
            SDL_Event e; int mx=0,my=0; SDL_GetMouseState(&mx,&my);
            while (SDL_PollEvent(&e)) {
                if (e.type==SDL_QUIT) { running=false; inMenu=false; }
                if (e.type==SDL_KEYDOWN && e.key.keysym.sym==SDLK_ESCAPE) { running=false; inMenu=false; }

                if (e.type==SDL_MOUSEBUTTONDOWN && e.button.button==SDL_BUTTON_LEFT) {
                    if (inside(btnExit,mx,my)) { running=false; inMenu=false; break; }

                    if (inside(btnNew, mx, my)) {
                        // Start fresh for a new project
                        ResetModelsAndUI();
                        ResetCounters();

                        // choose next available projectN.txt and ensure it exists
                        gProjectPath = NextProjectPath();
                        OpenOrCreateProjectFile(gProjectPath);

                        // (optional) write an initial empty file in phase-one format
                        save_circuit_to_file(gProjectPath, gCircuit);

                        // delete menu window before opening editor
                        if (fontBtn)   { TTF_CloseFont(fontBtn);   fontBtn   = nullptr; }
                        if (fontTitle) { TTF_CloseFont(fontTitle); fontTitle = nullptr; }
                        SDL_DestroyRenderer(renderer); renderer = nullptr;
                        SDL_DestroyWindow(window);     window   = nullptr;

                        ShowSchematicEditorWindow();
                        inMenu = false;
                        break;
                    }

                    if (inside(btnShow, mx, my)) {
                        // delete menu window before opening the list
                        if (fontBtn)   { TTF_CloseFont(fontBtn);   fontBtn   = nullptr; }
                        if (fontTitle) { TTF_CloseFont(fontTitle); fontTitle = nullptr; }
                        SDL_DestroyRenderer(renderer); renderer = nullptr;
                        SDL_DestroyWindow(window);     window   = nullptr;

                        ShowOpenProjectWindow();  // blocks; can launch editor or return
                        inMenu = false;           // recreate menu after we return
                        break;
                    }
                }
            }
            if (!inMenu) break;

            SDL_SetRenderDrawColor(renderer,14,18,28,255);
            SDL_RenderClear(renderer);

            boxRGBA(renderer, 0, 0, W, 80, 20,26,40,255);
            boxRGBA(renderer, 0, H-60, W, H, 20,26,40,255);

            roundedBoxRGBA(renderer, panel.x, panel.y, panel.x+panel.w, panel.y+panel.h, 16, 24,32,50,220);
            roundedRectangleRGBA(renderer, panel.x, panel.y, panel.x+panel.w, panel.y+panel.h, 16, 90,120,180,255);

            drawText(renderer, fontTitle, "Main Menu", panel.x + 24, panel.y + 20);

            auto drawBtn = [&](const SDL_Rect& r, const char* label){
                bool hot = inside(r, mx, my);
                Uint8 aFill = hot?255:200;
                roundedBoxRGBA(renderer, r.x, r.y, r.x+r.w, r.y+r.h, 12, hot?60:45, hot?160:120, hot?230:190, aFill);
                roundedRectangleRGBA(renderer, r.x, r.y, r.x+r.w, r.y+r.h, 12, 40,70,120,255);
                drawText(renderer, fontBtn, label, r.x + 20, r.y + (r.h/2 - 10));
            };

            drawBtn(btnNew,  "Create new project");
            drawBtn(btnShow, "Show existing schematics");
            drawBtn(btnExit, "Exit");

            SDL_RenderPresent(renderer);
        }

        if (fontBtn)   TTF_CloseFont(fontBtn);
        if (fontTitle) TTF_CloseFont(fontTitle);
        if (renderer) SDL_DestroyRenderer(renderer);
        if (window)   SDL_DestroyWindow(window);
    }

    SDL_Quit();
}



int main(int argc, char* argv[]) {
    ShowMainMenuWindow();
    return 0;
}
