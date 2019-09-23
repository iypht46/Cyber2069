﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Events;

public class HP : MonoBehaviour
{
    public UnityEvent dead;
    public float Maxhp;
    public float hp;
    public bool invincible = false;
    public bool isDead = false;

    SpriteRenderer sr;
    private UI ui;

    private void Awake()
    {
        sr = GetComponent<SpriteRenderer>();
        ui = GameObject.Find("UI").GetComponent<UI>();
    }

    void OnEnable()
    {
        sr.enabled = true;
        hp = Maxhp;
        isDead = false;
    }

    // Update is called once per frame
    void Update()
    {
        if(!isDead && hp <= 0)
        {
            isDead = true;
            dead.Invoke();
        }
    }

    public void TakeDamage(float damage)
    {
        if (!invincible)
        {
            if (this.CompareTag("Player"))
            {
                ui.ResetCombo();
            }
            hp -= damage;
        }
    }

    public IEnumerator Dead(float time)
    {
        sr.enabled = false;
        yield return new WaitForSeconds(time);
        gameObject.SetActive(false);
    }
}
