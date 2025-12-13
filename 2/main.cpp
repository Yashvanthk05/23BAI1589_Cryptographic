#include <bits/stdc++.h>
using namespace std;

// ================= N-gram model =================

struct NGramModel {
    unordered_map<string,double> logProb;
    int n;
};

NGramModel monoModel, biModel, triModel;

// ---------- basic text utilities ----------

string preprocess(const string &s) {
    string t;
    for (char ch : s) {
        if (isalpha((unsigned char)ch))
            t.push_back((char)tolower((unsigned char)ch));
    }
    return t;
}

string apply_key(const string &text, const string &key) {
    string out = text;
    for (char &ch : out) {
        if ('a' <= ch && ch <= 'z') {
            ch = key[ch - 'a'];
        }
    }
    return out;
}

unordered_map<string,long long> count_ngrams(const string &text, int n) {
    unordered_map<string,long long> cnt;
    if ((int)text.size() < n) return cnt;
    for (int i = 0; i + n <= (int)text.size(); ++i) {
        bool ok = true;
        for (int j = 0; j < n; ++j) {
            if (!(text[i+j] >= 'a' && text[i+j] <= 'z')) {
                ok = false;
                break;
            }
        }
        if (!ok) continue;
        string g = text.substr(i, n);
        cnt[g]++;
    }
    return cnt;
}

// ---------- n-gram model loading ----------

void normalize_counts(unordered_map<string,double> &raw, NGramModel &model) {
    double total = 0.0;
    for (auto &p : raw) total += p.second;
    double alpha = 0.1;
    int vocab = (int)raw.size();
    for (auto &p : raw) {
        double prob = (p.second + alpha) / (total + alpha * vocab);
        model.logProb[p.first] = log(prob);
    }
}

void load_english_models() {
    // NOTE: for good decryption, replace these toy counts
    // with full English frequency tables from external files.[web:5][web:14]

    // monograms
    {
        unordered_map<string,double> raw;
        raw["e"] = 12000; raw["t"] = 9000; raw["a"] = 8000;
        raw["o"] = 7500;  raw["i"] = 7000; raw["n"] = 6700;
        raw["s"] = 6300;  raw["h"] = 6100; raw["r"] = 6000;
        for (char c = 'a'; c <= 'z'; ++c) {
            string s(1,c);
            if (!raw.count(s)) raw[s] = 3000;
        }
        monoModel.n = 1;
        normalize_counts(raw, monoModel);
    }

    // bigrams (very small subset; extend this)
    {
        unordered_map<string,double> raw;
        raw["th"] = 15000;
        raw["he"] = 14000;
        raw["in"] = 12000;
        raw["er"] = 11000;
        raw["an"] = 10000;
        raw["re"] = 9000;
        biModel.n = 2;
        normalize_counts(raw, biModel);
    }

    // trigrams (subset; extend)
    {
        unordered_map<string,double> raw;
        raw["the"] = 20000;
        raw["and"] = 15000;
        raw["ing"] = 14000;
        triModel.n = 3;
        normalize_counts(raw, triModel);
    }
}

// ---------- scoring ----------

double score_with_model(const string &plain, const NGramModel &model) {
    auto counts = count_ngrams(plain, model.n);
    double score = 0.0;
    double floorVal = log(1e-9);
    for (auto &p : counts) {
        auto it = model.logProb.find(p.first);
        double lp = (it != model.logProb.end()) ? it->second : floorVal;
        score += lp * (double)p.second;
    }
    return score;
}

double mono_score(const string &plain) { return score_with_model(plain, monoModel); }
double bi_score(const string &plain)   { return score_with_model(plain, biModel); }
double tri_score(const string &plain)  { return score_with_model(plain, triModel); }

double combined_score(const string &plain) {
    double a = 1.0, b = 10.0, c = 40.0;
    return a * mono_score(plain)
         + b * bi_score(plain)
         + c * tri_score(plain);
}

// ---------- key utilities (no random_shuffle) ----------

string identity_key() {
    string k(26, 'a');
    for (int i = 0; i < 26; ++i) k[i] = 'a' + i;
    return k;
}

// Fisher–Yates based on rand()
string random_key() {
    string k = identity_key();
    for (int i = 25; i > 0; --i) {
        int j = rand() % (i + 1);
        swap(k[i], k[j]);
    }
    return k;
}

void mutate_key(string &key) {
    int a = rand() % 26;
    int b = rand() % 26;
    while (b == a) b = rand() % 26;
    swap(key[a], key[b]);
}

// ---------- hill climbing with multiple restarts ----------

typedef double (*ScoreFunc)(const string &);

string single_hill_climb(const string &cipher, ScoreFunc scorer,
                         int maxIter) {
    string bestKey = random_key();
    string bestPlain = apply_key(cipher, bestKey);
    double bestScore = scorer(bestPlain);

    for (int it = 0; it < maxIter; ++it) {
        string candKey = bestKey;
        mutate_key(candKey);
        string candPlain = apply_key(cipher, candKey);
        double candScore = scorer(candPlain);
        if (candScore > bestScore) {
            bestScore = candScore;
            bestKey = candKey;
            bestPlain = candPlain;
        }
    }
    return bestKey;
}

string multi_restart_hc(const string &cipher, ScoreFunc scorer,
                        int restarts, int iterPerRestart) {
    string globalKey;
    double globalBest = -1e300;
    bool first = true;

    for (int r = 0; r < restarts; ++r) {
        string k = single_hill_climb(cipher, scorer, iterPerRestart);
        string p = apply_key(cipher, k);
        double s = scorer(p);
        if (first || s > globalBest) {
            first = false;
            globalBest = s;
            globalKey = k;
        }
    }
    return globalKey;
}

// ---------- IO helpers ----------

string read_file_all(const string &path) {
    ifstream fin(path);
    stringstream buf;
    buf << fin.rdbuf();
    return buf.str();
}

void write_file(const string &path, const string &text) {
    ofstream fout(path);
    fout << text;
}

void print_key(const string &title, const string &key) {
    cout << "=== " << title << " ===\n";
    for (int i = 0; i < 26; ++i) {
        cout << char('a' + i) << " -> " << key[i] << "\n";
    }
    cout << "\n";
}

// ---------- main ----------

int main(int argc, char **argv) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <ciphertext_file>\n";
        return 1;
    }

    srand((unsigned)time(nullptr));

    string raw = read_file_all(argv[1]);
    string cipher = preprocess(raw);

    load_english_models();

    // monogram
    auto monoFun = [](const string &plain) { return mono_score(plain); };
    string keyMono = multi_restart_hc(cipher, monoFun, 20, 4000);
    string plainMono = apply_key(cipher, keyMono);
    write_file("decrypted_monogram.txt", plainMono);
    print_key("Monogram key", keyMono);

    // digram
    auto biFun = [](const string &plain) { return bi_score(plain); };
    string keyBi = multi_restart_hc(cipher, biFun, 20, 4000);
    string plainBi = apply_key(cipher, keyBi);
    write_file("decrypted_bigram.txt", plainBi);
    print_key("Bigram key", keyBi);

    // trigram
    auto triFun = [](const string &plain) { return tri_score(plain); };
    string keyTri = multi_restart_hc(cipher, triFun, 20, 4000);
    string plainTri = apply_key(cipher, keyTri);
    write_file("decrypted_trigram.txt", plainTri);
    print_key("Trigram key", keyTri);

    // combined
    auto combFun = [](const string &plain) { return combined_score(plain); };
    string keyComb = multi_restart_hc(cipher, combFun, 30, 6000);
    string plainComb = apply_key(cipher, keyComb);
    write_file("decrypted_combined.txt", plainComb);
    print_key("Combined key", keyComb);

    return 0;
}
