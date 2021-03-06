﻿// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once

#include "pch.h"
#include "common.h"

#include "TabView.g.h"
#include "TabView.properties.h"
#include "TabViewTabCloseRequestedEventArgs.g.h"
#include "TabViewTabDroppedOutsideEventArgs.g.h"
#include "TabViewTabDragStartingEventArgs.g.h"
#include "TabViewTabDragCompletedEventArgs.g.h"
#include "DispatcherHelper.h"

class TabViewTabCloseRequestedEventArgs :
    public winrt::implementation::TabViewTabCloseRequestedEventArgsT<TabViewTabCloseRequestedEventArgs>
{
public:
    TabViewTabCloseRequestedEventArgs(winrt::IInspectable const& item, winrt::TabViewItem tab) : m_item(item), m_tab(tab) {}

    winrt::IInspectable Item() { return m_item; }
    winrt::TabViewItem Tab() { return m_tab; }

private:
    winrt::IInspectable m_item{};
    winrt::TabViewItem m_tab{};
};

class TabViewTabDroppedOutsideEventArgs :
    public winrt::implementation::TabViewTabDroppedOutsideEventArgsT<TabViewTabDroppedOutsideEventArgs>
{
public:
    TabViewTabDroppedOutsideEventArgs(winrt::IInspectable const& item, winrt::TabViewItem tab) : m_item(item), m_tab(tab) {}

    winrt::IInspectable Item() { return m_item; }
    winrt::TabViewItem Tab() { return m_tab; }

private:
    winrt::IInspectable m_item{};
    winrt::TabViewItem m_tab{};
};

class TabViewTabDragStartingEventArgs :
    public winrt::implementation::TabViewTabDragStartingEventArgsT<TabViewTabDragStartingEventArgs>
{
public:
    TabViewTabDragStartingEventArgs(winrt::DragItemsStartingEventArgs const& args, winrt::IInspectable const& item, winrt::TabViewItem tab) : m_args(args), m_item(item), m_tab(tab) {}

    bool Cancel() { return m_args.Cancel(); }
    void Cancel(bool value) { m_args.Cancel(value); }
    winrt::DataPackage Data() { return m_args.Data(); }
    winrt::IInspectable Item() { return m_item; }
    winrt::TabViewItem Tab() { return m_tab; }

private:
    winrt::DragItemsStartingEventArgs m_args{};
    winrt::IInspectable m_item{};
    winrt::TabViewItem m_tab{};
};

class TabViewTabDragCompletedEventArgs :
    public winrt::implementation::TabViewTabDragCompletedEventArgsT<TabViewTabDragCompletedEventArgs>
{
public:
    TabViewTabDragCompletedEventArgs(winrt::DragItemsCompletedEventArgs const& args, winrt::IInspectable const& item, winrt::TabViewItem tab) : m_args(args), m_item(item), m_tab(tab) {}

    winrt::DataPackageOperation DropResult() { return m_args.DropResult(); }
    winrt::IInspectable Item() { return m_item; }
    winrt::TabViewItem Tab() { return m_tab; }

private:
    winrt::DragItemsCompletedEventArgs m_args{ nullptr };
    winrt::IInspectable m_item{};
    winrt::TabViewItem m_tab{};
};

class TabView :
    public ReferenceTracker<TabView, winrt::implementation::TabViewT>,
    public TabViewProperties
{

public:
    TabView();

    // IFrameworkElement
    void OnApplyTemplate();

    // IUIElement
    winrt::AutomationPeer OnCreateAutomationPeer();

    // From ListView
    winrt::DependencyObject ContainerFromItem(winrt::IInspectable const& item);
    winrt::DependencyObject ContainerFromIndex(int index);
    winrt::IInspectable ItemFromContainer(winrt::DependencyObject const& container);

    // Control
    void OnKeyDown(winrt::KeyRoutedEventArgs const& e);

    // Internal
    void OnTabWidthModePropertyChanged(const winrt::DependencyPropertyChangedEventArgs& args);
    void OnSelectedIndexPropertyChanged(const winrt::DependencyPropertyChangedEventArgs& args);
    void OnSelectedItemPropertyChanged(const winrt::DependencyPropertyChangedEventArgs& args);

    void OnItemsChanged(winrt::IInspectable const& item);
    void UpdateTabContent();

    void RequestCloseTab(winrt::TabViewItem const& item);

private:
    void OnLoaded(const winrt::IInspectable& sender, const winrt::RoutedEventArgs& args);
    void OnScrollViewerLoaded(const winrt::IInspectable& sender, const winrt::RoutedEventArgs& args);
    void OnAddButtonClick(const winrt::IInspectable& sender, const winrt::RoutedEventArgs& args);
    void OnScrollDecreaseClick(const winrt::IInspectable& sender, const winrt::RoutedEventArgs& args);
    void OnScrollIncreaseClick(const winrt::IInspectable& sender, const winrt::RoutedEventArgs& args);
    void OnSizeChanged(const winrt::IInspectable& sender, const winrt::SizeChangedEventArgs& args);

    void OnListViewLoaded(const winrt::IInspectable& sender, const winrt::RoutedEventArgs& args);
    void OnListViewSelectionChanged(const winrt::IInspectable& sender, const winrt::SelectionChangedEventArgs& args);

    void OnListViewDragItemsStarting(const winrt::IInspectable& sender, const winrt::DragItemsStartingEventArgs& args);
    void OnListViewDragItemsCompleted(const winrt::IInspectable& sender, const winrt::DragItemsCompletedEventArgs& args);
    void OnListViewDragOver(const winrt::IInspectable& sender, const winrt::DragEventArgs& args);
    void OnListViewDrop(const winrt::IInspectable& sender, const winrt::DragEventArgs& args);

    void OnCtrlF4Invoked(const winrt::KeyboardAccelerator& sender, const winrt::KeyboardAcceleratorInvokedEventArgs& args);
    void OnCtrlTabInvoked(const winrt::KeyboardAccelerator& sender, const winrt::KeyboardAcceleratorInvokedEventArgs& args);
    void OnCtrlShiftTabInvoked(const winrt::KeyboardAccelerator& sender, const winrt::KeyboardAcceleratorInvokedEventArgs& args);

    bool RequestCloseCurrentTab();
    bool SelectNextTab(int increment);

    void UpdateSelectedItem();
    void UpdateSelectedIndex();

    void UpdateTabWidths();

    void OnListViewGettingFocus(const winrt::IInspectable& sender, const winrt::GettingFocusEventArgs& args);

    int GetItemCount();

    winrt::TabViewItem FindTabViewItemFromDragItem(const winrt::IInspectable& item);

    tracker_ref<winrt::ColumnDefinition> m_leftContentColumn{ this };
    tracker_ref<winrt::ColumnDefinition> m_tabColumn{ this };
    tracker_ref<winrt::ColumnDefinition> m_addButtonColumn{ this };
    tracker_ref<winrt::ColumnDefinition> m_rightContentColumn{ this };

    tracker_ref<winrt::ListView> m_listView{ this };
    tracker_ref<winrt::ContentPresenter> m_tabContentPresenter{ this };
    tracker_ref<winrt::ContentPresenter> m_rightContentPresenter{ this };
    tracker_ref<winrt::Grid> m_tabContainerGrid{ this };
    tracker_ref<winrt::FxScrollViewer> m_scrollViewer{ this };
    tracker_ref<winrt::Button> m_addButton{ this };

    winrt::ListView::Loaded_revoker m_listViewLoadedRevoker{};
    winrt::Selector::SelectionChanged_revoker m_listViewSelectionChangedRevoker{};
    winrt::UIElement::GettingFocus_revoker m_listViewGettingFocusRevoker{};

    winrt::ListView::DragItemsStarting_revoker m_listViewDragItemsStartingRevoker{};
    winrt::ListView::DragItemsCompleted_revoker m_listViewDragItemsCompletedRevoker{};
    winrt::UIElement::DragOver_revoker m_listViewDragOverRevoker{};
    winrt::UIElement::Drop_revoker m_listViewDropRevoker{};

    winrt::FxScrollViewer::Loaded_revoker m_scrollViewerLoadedRevoker{};

    winrt::Button::Click_revoker m_addButtonClickRevoker{};

    winrt::RepeatButton::Click_revoker m_scrollDecreaseClickRevoker{};
    winrt::RepeatButton::Click_revoker m_scrollIncreaseClickRevoker{};

    DispatcherHelper m_dispatcherHelper{ *this };
};
