import random
import re
from collections import Counter
import math

# ---------------------------------------------------------
# CONFIGURATION & CONSTANTS
# ---------------------------------------------------------
# Standard English Frequencies (approximate order)
ENGLISH_FREQ_ORDER = "ETAOINSHRDLCUMWFGYPBVKJXQZ"

# Top English Digrams (Pairs)
ENGLISH_BIGRAMS = ['TH', 'HE', 'IN', 'ER', 'AN', 'RE', 'ON', 'AT', 'EN', 'ND', 
                   'TI', 'ES', 'OR', 'TE', 'OF', 'ED', 'IS', 'IT', 'AL', 'AR']

# Top English Trigrams (Triplets)
ENGLISH_TRIGRAMS = ['THE', 'AND', 'THA', 'ENT', 'ING', 'ION', 'TIO', 'FOR', 
                    'NDE', 'HAS', 'NCE', 'EDT', 'TIS', 'OFT', 'STH', 'MEN']

# ---------------------------------------------------------
# HELPER FUNCTIONS
# ---------------------------------------------------------
def read_file(filepath):
    """Reads the ciphertext from a file."""
    try:
        with open(filepath, 'r', encoding='utf-8') as f:
            text = f.read()
        # Clean text: keep only letters, convert to uppercase for analysis
        clean_text = re.sub(r'[^A-Z]', '', text.upper())
        return text, clean_text
    except FileNotFoundError:
        print(f"Error: File '{filepath}' not found.")
        return None, None

def get_mapping_table(key_map):
    """Formats the key mapping for the report."""
    sorted_map = sorted(key_map.items())
    table_str = "\n--- Key Mapping Table ---\n"
    table_str += "| Cipher | Plain |\n| :--- | :--- |\n"
    for cipher, plain in sorted_map:
        table_str += f"|   {cipher}    |   {plain}   |\n"
    return table_str

def apply_substitution(text, mapping):
    """Deciphers the text using the provided mapping."""
    result = ""
    for char in text:
        upper_char = char.upper()
        if upper_char in mapping:
            # Preserve case
            if char.isupper():
                result += mapping[upper_char]
            else:
                result += mapping[upper_char].lower()
        else:
            result += char
    return result

# ---------------------------------------------------------
# ALGORITHM 1: MONOGRAM ONLY
# ---------------------------------------------------------
def solve_monogram(ciphertext_clean, original_text):
    print("\n[+] Running Monogram Analysis...")
    
    # 1. Count frequencies
    counts = Counter(ciphertext_clean)
    sorted_cipher_chars = [pair[0] for pair in counts.most_common()]
    
    # 2. Map to English frequency order (ETAOIN...)
    mapping = {}
    
    # Fill knowns
    for i in range(min(len(sorted_cipher_chars), len(ENGLISH_FREQ_ORDER))):
        cipher_char = sorted_cipher_chars[i]
        plain_char = ENGLISH_FREQ_ORDER[i]
        mapping[cipher_char] = plain_char
        
    # Fill remaining letters (if any missing from text) with unused English letters
    used_plain = set(mapping.values())
    unused_plain = [c for c in ENGLISH_FREQ_ORDER if c not in used_plain]
    
    alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    for char in alphabet:
        if char not in mapping:
            if unused_plain:
                mapping[char] = unused_plain.pop(0)
            else:
                mapping[char] = '?' # Should not happen if texts are long enough

    decrypted = apply_substitution(original_text, mapping)
    return mapping, decrypted

# ---------------------------------------------------------
# ALGORITHM 2: DIGRAM ONLY (Heuristic)
# ---------------------------------------------------------
def solve_digram(ciphertext_clean, original_text):
    print("\n[+] Running Digram Analysis...")
    
    # 1. Count Digrams
    digrams = [ciphertext_clean[i:i+2] for i in range(len(ciphertext_clean)-1)]
    counts = Counter(digrams)
    top_cipher_digrams = [pair[0] for pair in counts.most_common(20)]
    
    mapping = {}
    used_plain = set()
    used_cipher = set()

    # 2. Heuristic Mapping
    # Try to map the most frequent cipher digram to "TH", 2nd to "HE", etc.
    # This is a greedy approach and may have conflicts.
    
    # Simple strategy: Just map the top few strictly to build a partial key
    # Realistically, Digram analysis is used to *refine* Monograms, but here is a standalone attempt.
    
    for i in range(min(len(top_cipher_digrams), len(ENGLISH_BIGRAMS))):
        c_digram = top_cipher_digrams[i]
        p_digram = ENGLISH_BIGRAMS[i]
        
        # Map first char
        if c_digram[0] not in mapping and p_digram[0] not in used_plain:
            mapping[c_digram[0]] = p_digram[0]
            used_plain.add(p_digram[0])
            used_cipher.add(c_digram[0])
            
        # Map second char
        if c_digram[1] not in mapping and p_digram[1] not in used_plain:
            mapping[c_digram[1]] = p_digram[1]
            used_plain.add(p_digram[1])
            used_cipher.add(c_digram[1])

    # Fill the rest with remaining monograms to make it readable
    remaining_cipher = [c for c in ENGLISH_FREQ_ORDER if c not in mapping]
    remaining_plain = [c for c in ENGLISH_FREQ_ORDER if c not in used_plain]
    
    for i, c in enumerate(remaining_cipher):
        if i < len(remaining_plain):
            mapping[c] = remaining_plain[i]
            
    decrypted = apply_substitution(original_text, mapping)
    return mapping, decrypted

# ---------------------------------------------------------
# ALGORITHM 3: TRIGRAM ONLY (Heuristic)
# ---------------------------------------------------------
def solve_trigram(ciphertext_clean, original_text):
    print("\n[+] Running Trigram Analysis...")
    
    # 1. Count Trigrams
    trigrams = [ciphertext_clean[i:i+3] for i in range(len(ciphertext_clean)-2)]
    counts = Counter(trigrams)
    top_cipher_trigrams = [pair[0] for pair in counts.most_common(20)]
    
    mapping = {}
    used_plain = set()
    
    # 2. Heuristic Mapping: Map top cipher trigram to "THE" immediately
    if top_cipher_trigrams:
        top_tri = top_cipher_trigrams[0]
        # Force mapping to THE
        mapping[top_tri[0]] = 'T'
        mapping[top_tri[1]] = 'H'
        mapping[top_tri[2]] = 'E'
        used_plain.update(['T', 'H', 'E'])

    # Try to map others (AND, ING...)
    for i in range(1, min(len(top_cipher_trigrams), len(ENGLISH_TRIGRAMS))):
        c_tri = top_cipher_trigrams[i]
        p_tri = ENGLISH_TRIGRAMS[i]
        
        for j in range(3):
            if c_tri[j] not in mapping and p_tri[j] not in used_plain:
                mapping[c_tri[j]] = p_tri[j]
                used_plain.add(p_tri[j])

    # Fill rest based on frequency fallback
    alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    unused_plain = [c for c in ENGLISH_FREQ_ORDER if c not in used_plain]
    
    for char in alphabet:
        if char not in mapping:
            if unused_plain:
                mapping[char] = unused_plain.pop(0)

    decrypted = apply_substitution(original_text, mapping)
    return mapping, decrypted

# ---------------------------------------------------------
# ALGORITHM 4: ALL COMBINATIONS (Hill Climbing)
# ---------------------------------------------------------
def score_text(text):
    """Scores text based on English trigram log-probabilities."""
    # Simplified: Higher score for common trigrams
    score = 0
    trigrams = [text[i:i+3] for i in range(len(text)-2)]
    for tri in trigrams:
        if tri in ENGLISH_TRIGRAMS:
            score += 10
        if tri[:2] in ENGLISH_BIGRAMS:
            score += 3
    return score

def solve_combined(ciphertext_clean, original_text):
    print("\n[+] Running Combined Analysis (Hill Climbing)...")
    
    # Start with Monogram frequency as a baseline key
    parent_map, _ = solve_monogram(ciphertext_clean, "")
    parent_decrypted = apply_substitution(ciphertext_clean, parent_map)
    parent_score = score_text(parent_decrypted)
    
    alphabet = list("ABCDEFGHIJKLMNOPQRSTUVWXYZ")
    
    # Iterative improvement
    iterations = 2000
    for i in range(iterations):
        # Create a child mapping by swapping two keys
        child_map = parent_map.copy()
        char1, char2 = random.sample(alphabet, 2)
        
        # Swap values
        child_map[char1], child_map[char2] = child_map[char2], child_map[char1]
        
        # Decrypt and Score
        child_decrypted = apply_substitution(ciphertext_clean, child_map)
        child_score = score_text(child_decrypted)
        
        # If child is better, keep it
        if child_score > parent_score:
            parent_score = child_score
            parent_map = child_map
            # Optional: Print progress
            # if i % 100 == 0: print(f"Iter {i}: Score {parent_score}")

    final_decrypted = apply_substitution(original_text, parent_map)
    return parent_map, final_decrypted

# ---------------------------------------------------------
# MAIN EXECUTION
# ---------------------------------------------------------
if __name__ == "__main__":
    # FILE